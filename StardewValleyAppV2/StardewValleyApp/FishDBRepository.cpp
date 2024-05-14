#include "FishDBRepository.h"
#include "sqlite3.h"

using namespace std;

FishDBRepository::FishDBRepository(const string& databasePath) : databasePath(databasePath) {}

Fish FishDBRepository::findOne(long id) const {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT name, season, weather, location, start_catching_hour, end_catching_hour, difficulty, is_caught FROM Fish WHERE id = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, id);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_ROW) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Extract data
	string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
	string season = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
	string weather = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
	string location = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
	long startCatchingHour = sqlite3_column_int(statement, 4);
	long endCatchingHour = sqlite3_column_int(statement, 5);
	long difficult = sqlite3_column_int(statement, 6);
	long isCaught = sqlite3_column_int(statement, 7) != 0;

	// Clean up and return result
	sqlite3_finalize(statement);
	sqlite3_close(db);
	return Fish(name, season, weather, location, startCatchingHour, endCatchingHour, difficult, isCaught);
}

Fish FishDBRepository::findOneByName(string& name) const {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, season, weather, location, start_catching_hour, end_catching_hour, difficulty, is_caught FROM Fish WHERE name = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Bind parameters
	sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_ROW) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Extract data
	long id = sqlite3_column_int(statement, 0);
	string season = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
	string weather = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
	string location = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
	long startCatchingHour = sqlite3_column_int(statement, 4);
	long endCatchingHour = sqlite3_column_int(statement, 5);
	long difficult = sqlite3_column_int(statement, 6);
	long isCaught = sqlite3_column_int(statement, 7) != 0;

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return Fish(id, name, season, weather, location, startCatchingHour, endCatchingHour, difficult, isCaught);
}

vector<Fish> FishDBRepository::findAll() const {
	vector<Fish> allFish;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, name, season, weather, location, start_catching_hour, end_catching_hour, difficulty, is_caught FROM Fish";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		std::string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		std::string season = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		std::string weather = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		std::string location = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		long startCatchingHour = sqlite3_column_int(statement, 5);
		long endCatchingHour = sqlite3_column_int(statement, 6);
		long difficulty = sqlite3_column_int(statement, 7);
		bool isCaught = sqlite3_column_int(statement, 8) != 0;

		allFish.push_back(Fish(id, name, season, weather, location, startCatchingHour, endCatchingHour, difficulty, isCaught));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}

void FishDBRepository::save(Fish& fish) {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "INSERT INTO Fish (name, season, weather, location, start_catching_hour, end_catching_hour, difficulty, is_caught) VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Bind parameters
	sqlite3_bind_text(statement, 1, fish.getName().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 2, fish.getSeason().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 3, fish.getWeather().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 4, fish.getLocation().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(statement, 5, fish.getStartCatchingHour());
	sqlite3_bind_int(statement, 6, fish.getEndCatchingHour());
	sqlite3_bind_int(statement, 7, fish.getDifficulty());
	sqlite3_bind_int(statement, 8, fish.getIsCaught() ? 1 : 0);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);
}

void FishDBRepository::remove(long id) {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "DELETE FROM Fish WHERE id = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Bind parameter
	sqlite3_bind_int(statement, 1, id);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);
}

void FishDBRepository::update(const Fish& fish) {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "UPDATE Fish SET name = ?, season = ?, weather = ?, location = ?, start_catching_hour = ?, end_catching_hour = ?, difficulty = ?, is_caught = ? WHERE name = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Bind parameters
	sqlite3_bind_text(statement, 1, fish.getName().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 2, fish.getSeason().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 3, fish.getWeather().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 4, fish.getLocation().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(statement, 5, fish.getStartCatchingHour());
	sqlite3_bind_int(statement, 6, fish.getEndCatchingHour());
	sqlite3_bind_int(statement, 7, fish.getDifficulty());
	sqlite3_bind_int(statement, 8, fish.getIsCaught() ? 1 : 0);
	sqlite3_bind_text(statement, 9, fish.getName().c_str(), -1, SQLITE_STATIC);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);
}