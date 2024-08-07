#include "FishDBRepository.h"

using namespace std;



/*
	Constructor for the FishDBRepository class.
	Initializes the databasePath field with the given database path.
	Params:
		databasePath - the path to the database
*/
FishDBRepository::FishDBRepository(const string& databasePath) : databasePath(databasePath) {
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
	}
	else {
		sqlite3_close(db);
	}
}



/*
	Function that returns a Fish object from the database with the given id and the specific username of the logged user.
	If the fish is not found, an empty Fish object is returned.
	Params:
		id - the id of the fish
		username - the username of the logged user
*/
Fish FishDBRepository::findOne(long id, const string& username) const {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return Fish();
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT name, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish WHERE id = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return Fish();
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, id);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_ROW) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return Fish();
	}

	// Extract data
	string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
	vector<string> seasons = getSeasonsByFishId(db, id);
	vector<string> weathers = getWeathersByFishId(db, id);
	vector<string> locations = getLocationsByFishId(db, id);
	long startCatchingHour = sqlite3_column_int(statement, 1);
	long endCatchingHour = sqlite3_column_int(statement, 2);
	long difficulty = sqlite3_column_int(statement, 3);
	string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
	long isCaught = getIsCaughtByFishId(db, id, username);
	const void* imageBlob = sqlite3_column_blob(statement, 5);
	int imageSize = sqlite3_column_bytes(statement, 5);
	std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

	// Clean up and return result
	sqlite3_finalize(statement);
	sqlite3_close(db);
	return Fish(name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, isCaught, image);
}



/*
	Function that returns a Fish object from the database with the given name and the specific username of the logged user.
	If the fish is not found, an empty Fish object is returned.
	Params:
		name - the name of the fish
		username - the username of the logged user
*/
Fish FishDBRepository::findOneByName(const string& name, const string& username) const {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return Fish();
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish WHERE name = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return Fish();
	}

	// Bind parameters
	sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_ROW) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return Fish();
	}

	// Extract data
	long id = sqlite3_column_int(statement, 0);
	vector<string> seasons = getSeasonsByFishId(db, id);
	vector<string> weathers = getWeathersByFishId(db, id);
	vector<string> locations = getLocationsByFishId(db, id);
	long startCatchingHour = sqlite3_column_int(statement, 1);
	long endCatchingHour = sqlite3_column_int(statement, 2);
	long difficulty = sqlite3_column_int(statement, 3);
	string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
	long isCaught = getIsCaughtByFishId(db, id, username);
	const void* imageBlob = sqlite3_column_blob(statement, 5);
	int imageSize = sqlite3_column_bytes(statement, 5);
	std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return Fish(id, name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, isCaught, image);
}



/*
	Function that returns all the Fish objects from the database.
	Params:
		username - the username of the logged user
*/
vector<Fish> FishDBRepository::findAll(const string& username) const {
	vector<Fish> allFish;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return allFish;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, name, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return allFish;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long startCatchingHour = sqlite3_column_int(statement, 2);
		long endCatchingHour = sqlite3_column_int(statement, 3);
		long difficulty = sqlite3_column_int(statement, 4);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long isCaught = getIsCaughtByFishId(db, id, username);
		const void* imageBlob = sqlite3_column_blob(statement, 6);
		int imageSize = sqlite3_column_bytes(statement, 6);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		allFish.push_back(Fish(id, name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, isCaught, image));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}



/*
	Function that saves a Fish object to the database.
	Params:
		fish - the Fish object to be saved
*/
void FishDBRepository::save(Fish& fish) {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "INSERT INTO Fish (name, start_catching_hour, end_catching_hour, difficulty, movement, image) VALUES (?, ?, ?, ?, ?, ?)";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return;
	}

	// Bind parameters
	sqlite3_bind_text(statement, 1, fish.getName().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(statement, 2, fish.getStartCatchingHour());
	sqlite3_bind_int(statement, 3, fish.getEndCatchingHour());
	sqlite3_bind_int(statement, 4, fish.getDifficulty());
	sqlite3_bind_text(statement, 5, fish.getMovement().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_blob(statement, 6, fish.getImage().data(), fish.getImage().size(), SQLITE_STATIC);

	// Execute query
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return;
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);
}



/*
	Function that removes a Fish object from the database with the given id.
	Params:
		id - the id of the fish to be removed
*/
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



/*
	Function that updates a Fish object in the database.
	Params:
		fish - the Fish object to be updated
*/
void FishDBRepository::update(const Fish& fish) {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return;
	}

	// Begin transaction
	sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "UPDATE Fish SET start_catching_hour = ?, end_catching_hour = ?, difficulty = ?, is_caught = ? WHERE name = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return;
	}

	// Bind parameters
	sqlite3_bind_text(statement, 1, fish.getName().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(statement, 5, fish.getStartCatchingHour());
	sqlite3_bind_int(statement, 6, fish.getEndCatchingHour());
	sqlite3_bind_int(statement, 7, fish.getDifficulty());
	sqlite3_bind_int(statement, 8, fish.getIsCaught() ? 1 : 0);
	sqlite3_bind_text(statement, 9, fish.getName().c_str(), -1, SQLITE_STATIC);

	// Execute query
	rc = sqlite3_step(statement);
	sqlite3_finalize(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
		sqlite3_close(db);
		return;  // Add a return statement to exit the function on error
	}

	// Update related tables
	updateRelatedTable(db, fish.getName(), fish.getSeason(), "Fish_Season", "season_id");
	updateRelatedTable(db, fish.getName(), fish.getWeather(), "Fish_Weather", "weather_id");
	updateRelatedTable(db, fish.getName(), fish.getLocation(), "Fish_Location", "location_id");

	// Commit transaction
	sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
	sqlite3_close(db);
}



/*
	Function that saves an image to the database for a Fish object with the given id.
	Params:
		fishId - the id of the fish
		image - the image to be saved
*/
void FishDBRepository::saveImage(long fishId, const std::vector<char>& image) {
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	sqlite3_stmt* statement;
	const char* query = "UPDATE Fish SET image = ? WHERE id = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_bind_blob(statement, 1, image.data(), image.size(), SQLITE_STATIC);
	sqlite3_bind_int(statement, 2, fishId);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_finalize(statement);
	sqlite3_close(db);
}



/*
	Function that returns an image from the database for a Fish object with the given id.
	Params:
		fishId - the id of the fish
*/
std::vector<char> FishDBRepository::getImage(long fishId) const {
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	sqlite3_stmt* statement;
	const char* query = "SELECT image FROM Fish WHERE id = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_bind_int(statement, 1, fishId);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_ROW) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	const void* imageBlob = sqlite3_column_blob(statement, 0);
	int imageSize = sqlite3_column_bytes(statement, 0);
	std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

	sqlite3_finalize(statement);
	sqlite3_close(db);
	return image;
}



// NOT TESTED!!!
vector<string> FishDBRepository::findAllWeathers() const noexcept {
	vector<string> weathers;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return weathers;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT w.name FROM Weathers w";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return weathers;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		const unsigned char* name = sqlite3_column_text(statement, 0);
		weathers.push_back(reinterpret_cast<const char*>(name));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return weathers;
}



// NOT TESTED!!!
vector<string> FishDBRepository::findAllSeasons() const noexcept {
	vector<string> seasons;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return seasons;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT s.name FROM Seasons s";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return seasons;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		const unsigned char* name = sqlite3_column_text(statement, 0);
		seasons.push_back(reinterpret_cast<const char*>(name));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return seasons;
}



// NOT TESTED!!!
vector<string> FishDBRepository::findAllLocations() const noexcept {
	vector<string> locations;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return locations;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT l.name FROM FishLocations l";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return locations;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		const unsigned char* name = sqlite3_column_text(statement, 0);
		locations.push_back(reinterpret_cast<const char*>(name));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return locations;
}



/*
	NOT TESTED!!!
	Function that returns all the Fish objects by a specific weather value from the database.
	Params:
		username - the username of the logged user
		weather - the weather value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllByWeather(const string& username, const string& weather) const noexcept {
	vector<Fish> allFish;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return allFish;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, name, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return allFish;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long startCatchingHour = sqlite3_column_int(statement, 2);
		long endCatchingHour = sqlite3_column_int(statement, 3);
		long difficulty = sqlite3_column_int(statement, 4);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long isCaught = getIsCaughtByFishId(db, id, username);
		const void* imageBlob = sqlite3_column_blob(statement, 6);
		int imageSize = sqlite3_column_bytes(statement, 6);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		if (find(weathers.begin(), weathers.end(), weather) != weathers.end()) {
			allFish.push_back(Fish(id, name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, isCaught, image));
		}
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}



/*
	NOT TESTED!!!
	Function that returns all the Fish objects by a specific season value from the database.
	Params:
		username - the username of the logged user
		season - the season value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllBySeason(const string& username, const string& season) const noexcept {
	vector<Fish> allFish;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return allFish;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, name, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return allFish;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long startCatchingHour = sqlite3_column_int(statement, 2);
		long endCatchingHour = sqlite3_column_int(statement, 3);
		long difficulty = sqlite3_column_int(statement, 4);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long isCaught = getIsCaughtByFishId(db, id, username);
		const void* imageBlob = sqlite3_column_blob(statement, 6);
		int imageSize = sqlite3_column_bytes(statement, 6);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		if (find(seasons.begin(), seasons.end(), season) != seasons.end()) {
			allFish.push_back(Fish(id, name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, isCaught, image));
		}
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}



/*
	NOT TESTED!!!
	Function that returns all the Fish objects by a specific location value from the database.
	Params:
		username - the username of the logged user
		selocationason - the location value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllByLocation(const string& username, const string& location) const noexcept {
	vector<Fish> allFish;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return allFish;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, name, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return allFish;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long startCatchingHour = sqlite3_column_int(statement, 2);
		long endCatchingHour = sqlite3_column_int(statement, 3);
		long difficulty = sqlite3_column_int(statement, 4);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long isCaught = getIsCaughtByFishId(db, id, username);
		const void* imageBlob = sqlite3_column_blob(statement, 6);
		int imageSize = sqlite3_column_bytes(statement, 6);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		if (find(locations.begin(), locations.end(), location) != locations.end()) {
			allFish.push_back(Fish(id, name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, isCaught, image));
		}
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}















/*
	Helper function that returns the seasons for a Fish object with the given id.
	Params:
		db - the database connection
		fishId - the id of the fish
	return - vector<string> - the seasons for the fish
*/
vector<string> FishDBRepository::getSeasonsByFishId(sqlite3* db, long fishId) const {
	vector<string> seasons;
	sqlite3_stmt* statement;
	const char* query = "SELECT s.name FROM Seasons s JOIN Fish_Season fs ON s.id = fs.season_id WHERE fs.fish_id = ?";
	int rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, fishId);

		while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
			const char* season = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
			if (season) {
				seasons.push_back(season);
				std::cout << "Found season: " << season << std::endl; // Debug logging
			}
			else {
				std::cout << "Season is null for fishId: " << fishId << std::endl; // Debug logging
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
	}
	return seasons;
}



/*
	Helper function that returns the weathers for a Fish object with the given id.
	Params:
		db - the database connection
		fishId - the id of the fish
	return - vector<string> - the weathers for the fish
*/
vector<string> FishDBRepository::getWeathersByFishId(sqlite3* db, long fishId) const {
	vector<string> weathers;
	sqlite3_stmt* statement;
	const char* query = "SELECT w.name FROM Weathers w JOIN Fish_Weather fw ON w.id = fw.weather_id WHERE fw.fish_id = ?";
	int rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, fishId);

		while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
			const char* weather = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
			if (weather) {
				weathers.push_back(weather);
				std::cout << "Found weather: " << weather << std::endl; // Debug logging
			}
			else {
				std::cout << "Weather is null for fishId: " << fishId << std::endl; // Debug logging
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
	}
	return weathers;
}



/*
	Helper function that returns the locations for a Fish object with the given id.
	Params:
		db - the database connection
		fishId - the id of the fish
	return - vector<string> - the locations for the fish
*/
vector<string> FishDBRepository::getLocationsByFishId(sqlite3* db, long fishId) const {
	vector<string> locations;
	sqlite3_stmt* statement;
	const char* query = "SELECT l.name FROM FishLocations l JOIN Fish_FishLocation fl ON l.id = fl.location_id WHERE fl.fish_id = ?";
	int rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, fishId);

		while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
			const char* location = reinterpret_cast<const char*>(sqlite3_column_text(statement, 0));
			if (location) {
				locations.push_back(location);
				std::cout << "Found location: " << location << std::endl; // Debug logging
			}
			else {
				std::cout << "Location is null for fishId: " << fishId << std::endl; // Debug logging
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
	}
	return locations;
}



/*
	Helper function that returns whether a Fish object with the given id is caught by the user with the given username.
	Params:
		db - the database connection
		fishId - the id of the fish
		username - the username of the user
*/
bool FishDBRepository::getIsCaughtByFishId(sqlite3* db, long fishId, const string& username) const {
	bool isCaught = false;
	sqlite3_stmt* statement;
	const char* query = "SELECT is_caught FROM Caught_Fish WHERE fish_id = ? AND user_id = ?";
	int rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, fishId);
		sqlite3_bind_text(statement, 2, username.c_str(), -1, SQLITE_STATIC);
		rc = sqlite3_step(statement);

		if (rc == SQLITE_ROW) {
			isCaught = sqlite3_column_int(statement, 0);
		}

		sqlite3_finalize(statement);
	}
	return isCaught;
}



void FishDBRepository::updateRelatedTable(sqlite3* db, const std::string& fishName, const std::vector<std::string>& items, const std::string& tableName, const std::string& itemIdColumn) {
	// Delete existing entries
	sqlite3_stmt* statement;
	std::string deleteQuery = "DELETE FROM " + tableName + " WHERE fish_name = ?";
	int rc = sqlite3_prepare_v2(db, deleteQuery.c_str(), -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(statement, 1, fishName.c_str(), -1, SQLITE_STATIC);
		sqlite3_step(statement);
		sqlite3_finalize(statement);
	}

	// Insert new entries
	std::string insertQuery = "INSERT INTO " + tableName + " (fish_name, " + itemIdColumn + ") VALUES (?, ?)";
	for (const auto& item : items) {
		rc = sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &statement, nullptr);
		if (rc == SQLITE_OK) {
			sqlite3_bind_text(statement, 1, fishName.c_str(), -1, SQLITE_STATIC);
			sqlite3_bind_text(statement, 2, item.c_str(), -1, SQLITE_STATIC);
			sqlite3_step(statement);
			sqlite3_finalize(statement);
		}
	}
}