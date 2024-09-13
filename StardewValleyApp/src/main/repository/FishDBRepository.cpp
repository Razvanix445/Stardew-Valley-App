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
		userId - the id of the logged user
*/
Fish FishDBRepository::findOne(long id, const long userId) const {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return Fish();
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT name, description, category, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish WHERE id = ?";
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
	string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
	string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
	vector<string> seasons = getSeasonsByFishId(db, id);
	vector<string> weathers = getWeathersByFishId(db, id);
	vector<string> locations = getLocationsByFishId(db, id);
	string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
	string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
	long difficulty = sqlite3_column_int(statement, 5);
	string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
	long isCaught = getIsCaughtByFishId(db, id, userId);
	long isFavorite = getIsFavoriteByFishId(db, id, userId);
	const void* imageBlob = sqlite3_column_blob(statement, 7);
	int imageSize = sqlite3_column_bytes(statement, 7);
	std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

	// Clean up and return result
	sqlite3_finalize(statement);
	sqlite3_close(db);
	return Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image);
}



/*
	Function that returns a Fish object from the database with the given name and the specific username of the logged user.
	If the fish is not found, an empty Fish object is returned.
	Params:
		name - the name of the fish
		userId - the id of the logged user
*/
Fish FishDBRepository::findOneByName(const string& name, const long userId) const {
	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return Fish();
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, category, description, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish WHERE name = ?";
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
	string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
	string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
	vector<string> seasons = getSeasonsByFishId(db, id);
	vector<string> weathers = getWeathersByFishId(db, id);
	vector<string> locations = getLocationsByFishId(db, id);
	string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
	string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
	long difficulty = sqlite3_column_int(statement, 5);
	string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 6));
	long isCaught = getIsCaughtByFishId(db, id, userId);
	long isFavorite = getIsFavoriteByFishId(db, id, userId);
	const void* imageBlob = sqlite3_column_blob(statement, 7);
	int imageSize = sqlite3_column_bytes(statement, 7);
	std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image);
}



/*
	Function that returns all the Fish objects from the database.
	Params:
		userId - the id of the logged user
*/
vector<Fish> FishDBRepository::findAll(const long userId) const {
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
	const char* query = "SELECT id, name, category, description, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
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
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		allFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
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
	sqlite3_bind_text(statement, 2, fish.getStartCatchingHour().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 3, fish.getEndCatchingHour().c_str(), -1, SQLITE_STATIC);
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
Fish FishDBRepository::update(const Fish& fish, const long userId) {
	qDebug() << "Updating fish in database: " << QString::fromStdString(fish.toString());

	sqlite3* db;
	sqlite3_stmt* stmt;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return Fish();
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return Fish();
	}

	// Update Fish table
	std::string fishUpdateQuery = "UPDATE Fish SET name = ?, category = ?, description = ?, start_catching_hour = ?, end_catching_hour = ?, difficulty = ?, movement = ? WHERE id = ?";
	rc = sqlite3_prepare_v2(db, fishUpdateQuery.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to prepare fishUpdateQuery: " << sqlite3_errmsg(db);
		sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
		sqlite3_close(db);
		return Fish();
	}

	sqlite3_bind_text(stmt, 1, fish.getName().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, fish.getCategory().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 3, fish.getDescription().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 4, fish.getStartCatchingHour().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 5, fish.getEndCatchingHour().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 6, fish.getDifficulty());
	sqlite3_bind_text(stmt, 7, fish.getMovement().c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 8, static_cast<int>(fish.getId()));

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		qDebug() << "Failed to update Fish table: " << sqlite3_errmsg(db);
		sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
		sqlite3_close(db);
		return Fish();
	}

	// Update Users_Fish table
	std::string usersFishUpdateQuery = "UPDATE Users_Fish SET is_caught = ?, is_favorite = ? WHERE user_id = ? AND fish_id = ?";
	rc = sqlite3_prepare_v2(db, usersFishUpdateQuery.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to prepare usersFishUpdateQuery: " << sqlite3_errmsg(db);
		sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
		sqlite3_close(db);
		return Fish();
	}

	sqlite3_bind_int(stmt, 1, fish.getIsCaught() ? 1 : 0);
	sqlite3_bind_int(stmt, 2, fish.getIsFavorite() ? 1 : 0);
	sqlite3_bind_int(stmt, 3, userId);
	sqlite3_bind_int(stmt, 4, fish.getId());

	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		qDebug() << "Failed to update Users_Fish table: " << sqlite3_errmsg(db);
		sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
		sqlite3_close(db);
		return Fish();
	}

	// Commit transaction
	rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to commit transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return Fish();
	}

	sqlite3_close(db);
	return fish;
}



/*
	Function that saves an image to the database for a Fish object with the given id.
	Params:
		fishId - the id of the fish
		image - the image to be saved
*/
void FishDBRepository::saveImage(long fishId, const vector<char>& image) {
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



/* Temporary Function */
void FishDBRepository::saveUserImage(long userId, const std::vector<char>& image) {
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	sqlite3_stmt* statement;
	const char* query = "UPDATE Users SET image = ? WHERE id = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_bind_blob(statement, 1, image.data(), image.size(), SQLITE_STATIC);
	sqlite3_bind_int(statement, 2, userId);
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_finalize(statement);
	sqlite3_close(db);
}



/*
	Function that saves an image to the database into the Images table.
	Params:
		name - the name of the image
		image - the image to be saved
*/
void FishDBRepository::saveImageToImages(const string& name, const vector<char>& image) {
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	sqlite3_stmt* statement;
	const char* query = "INSERT INTO Images (name, image) VALUES (?, ?)";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_blob(statement, 2, image.data(), image.size(), SQLITE_STATIC);
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
vector<char> FishDBRepository::getImage(long fishId) const {
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



/*
	Function that returns an image from the Images table from the database.
	Params:
		name - the name of the image
*/
std::vector<char> FishDBRepository::getImageFromImages(const string& name) const {
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	sqlite3_stmt* statement;
	const char* query = "SELECT image FROM Images WHERE name = ?";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		sqlite3_finalize(statement);
		sqlite3_close(db);
	}

	sqlite3_bind_text(statement, 1, name.c_str(), -1, SQLITE_STATIC);
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



/*
	Function that returns all the images from the Images and Fish tables in the database.
*/
QMap<QString, QPixmap> FishDBRepository::getAllImages() const {
	QMap<QString, QPixmap> images;
	QPixmap pixmap;

	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Preparing the SQL statement
	sqlite3_stmt* statement;
	const char* ImagesQuery = "SELECT i.name, i.image FROM Images i";
	rc = sqlite3_prepare_v2(db, ImagesQuery, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return images;
	}

	
	// Execute query for Images table
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		const unsigned char* name = sqlite3_column_text(statement, 0);

		const void* imageBlob = sqlite3_column_blob(statement, 1);
		int imageSize = sqlite3_column_bytes(statement, 1);
		const vector<char> imageData(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);
		QImage image;
		if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
			pixmap = QPixmap::fromImage(image);
		}
		else {
			qWarning() << "Failed to load image from given data!";
		}

		images.insert(QString::fromStdString(reinterpret_cast<const char*>(name)), pixmap);
	}


	// Execute query for Fish table
	const char* fishQuery = "SELECT f.name, f.image FROM Fish f";
	rc = sqlite3_prepare_v2(db, fishQuery, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return images;
	}


	// Execute query for Images table
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		const unsigned char* name = sqlite3_column_text(statement, 0);

		const void* imageBlob = sqlite3_column_blob(statement, 1);
		int imageSize = sqlite3_column_bytes(statement, 1);
		const vector<char> imageData(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);
		QImage image;
		if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
			pixmap = QPixmap::fromImage(image);
		}
		else {
			qWarning() << "Failed to load image from given data!";
		}

		images.insert("Fish_" + QString::fromStdString(reinterpret_cast<const char*>(name)), pixmap);
	}

	sqlite3_finalize(statement);
	sqlite3_close(db);
	return images;
}



/*
	Function that returns all the users from the database.
*/
vector<User> FishDBRepository::findAllUsers() const {
	vector<User> users;
	QPixmap pixmap;

	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
	}

	// Preparing the SQL statement
	sqlite3_stmt* statement;
	const char* usersQuery = "SELECT u.id, u.name, u.image FROM Users u";
	rc = sqlite3_prepare_v2(db, usersQuery, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return users;
	}


	// Execute query for Users table
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		long id = sqlite3_column_int(statement, 0);

		const unsigned char* name = sqlite3_column_text(statement, 1);

		const void* imageBlob = sqlite3_column_blob(statement, 2);
		int imageSize = sqlite3_column_bytes(statement, 2);
		const vector<char> imageData(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);
		QImage image;
		if (image.loadFromData(reinterpret_cast<const uchar*>(imageData.data()), imageData.size())) {
			pixmap = QPixmap::fromImage(image);
		}
		else {
			qWarning() << "Failed to load image from given data!";
		}
		users.push_back(User(id, reinterpret_cast<const char*>(name), pixmap));
	}

	sqlite3_finalize(statement);
	sqlite3_close(db);
	return users;
}



/*
	Function that returns all the weathers from the database (Weathers Table).
*/
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



/*
	Function that returns all the seasons from the database (Seasons Table).
*/
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



/*
	Function that returns all the locations from the database (FishLocations Table).
*/
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
	Function that returns all the Fish objects by a specific weather value from the database.
	Params:
		userId - the id of the logged user
		weather - the weather value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllByWeather(const long userId, const string& weather) const noexcept {
	vector<Fish> allFish;

	// Open connection to the database
	sqlite3* db;
	int rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_close(db);
		return allFish;
	}

	// Prepare SQL statement
	sqlite3_stmt* statement;
	const char* query = "SELECT id, name, category, description, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return allFish;
	}

	// Execute query
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		if (find(weathers.begin(), weathers.end(), weather) != weathers.end()) {
			allFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
		}
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}



/*
	Function that returns all the Fish objects by a specific season value from the database.
	Params:
		userId - the id of the logged user
		season - the season value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllBySeason(const long userId, const string& season) const noexcept {
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
	const char* query = "SELECT id, name, category, description, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
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
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		if (find(seasons.begin(), seasons.end(), season) != seasons.end()) {
			allFish.push_back(Fish(id, category, description, name, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
		}
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}



/*
	Function that returns all the Fish objects by a specific location value from the database.
	Params:
		userId - the id of the logged user
		selocationason - the location value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllByLocation(const long userId, const string& location) const noexcept {
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
	const char* query = "SELECT id, name, category, description, start_catching_hour, end_catching_hour, difficulty, movement, image FROM Fish";
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
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		if (find(locations.begin(), locations.end(), location) != locations.end()) {
			allFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
		}
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return allFish;
}



string FishDBRepository::toLowerCase(const string& str) const {
	string lowerStr = str;
	transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), [](unsigned char c) {
		return tolower(c);
		});
	return lowerStr;
}


/*
	Function that returns all the Fish objects filtered by a specific input from the database.
	Params:
		userId - the id of the logged user
		input - the input value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllFiltered(const long userId, const string& input) const {
	vector<Fish> filteredFish;

	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return filteredFish;
	}


	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return filteredFish;
	}

	string lowerInput = toLowerCase(input);

	const char* query = R"SQL(
        SELECT f.id, f.name, f.category, f.description, f.start_catching_hour, f.end_catching_hour, f.difficulty, f.movement, f.image
        FROM Fish f
        JOIN Users_Fish uf ON f.id = uf.fish_id
        LEFT JOIN Fish_Weather fw ON f.id = fw.fish_id
        LEFT JOIN Weathers w ON fw.weather_id = w.id
        LEFT JOIN Fish_Season fs ON f.id = fs.fish_id
        LEFT JOIN Seasons s ON fs.season_id = s.id
        LEFT JOIN Fish_FishLocation fl ON f.id = fl.fish_id
        LEFT JOIN FishLocations l ON fl.location_id = l.id
        WHERE uf.user_id = ?
        AND (
            LOWER(f.name) LIKE '%' || ? || '%' OR
            LOWER(w.name) LIKE '%' || ? || '%' OR
            LOWER(s.name) LIKE '%' || ? || '%' OR
            LOWER(l.name) LIKE '%' || ? || '%'
		)
        GROUP BY f.id
    )SQL";
	
	rc = sqlite3_prepare_v2(db, query, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, userId);
		sqlite3_bind_text(statement, 2, lowerInput.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 3, lowerInput.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 4, lowerInput.c_str(), -1, SQLITE_TRANSIENT);
		sqlite3_bind_text(statement, 5, lowerInput.c_str(), -1, SQLITE_TRANSIENT);

		while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
			int id = sqlite3_column_int(statement, 0);
			string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
			string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
			string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
			string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
			string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
			long difficulty = sqlite3_column_int(statement, 6);
			string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
			const void* imageBlob = sqlite3_column_blob(statement, 8);
			int imageSize = sqlite3_column_bytes(statement, 8);
			std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

			vector<string> seasons = getSeasonsByFishId(db, id);
			vector<string> weathers = getWeathersByFishId(db, id);
			vector<string> locations = getLocationsByFishId(db, id);
			long isCaught = getIsCaughtByFishId(db, id, userId);
			long isFavorite = getIsFavoriteByFishId(db, id, userId);

			filteredFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
		}

		sqlite3_finalize(statement);
	}
	else {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
	}

	// Commit transaction
	rc = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to commit transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return filteredFish;
	}

	sqlite3_close(db);
	return filteredFish;
}



/*
	Function that returns all the Fish objects from the database that have a specific season, weather and location.
	If the input for season, weather or location has "All (No Filter) value", it is not taken into consideration for filtering.
	Params:
		userId - the id of the logged user
		season - the season value for filtering the fish
		weather - the weather value for filtering the fish
		location - the location value for filtering the fish
*/
vector<Fish> FishDBRepository::findAllBySeasonWeatherLocation(const long userId, const string& season, const string& weather, const string& location) const noexcept {
	vector<Fish> filteredFish;
	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return filteredFish;
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return filteredFish;
	}

	// Prepare SQL query
	string query = R"(
        SELECT f.id, f.name, f.category, f.description, f.start_catching_hour, f.end_catching_hour, f.difficulty, f.movement, f.image
        FROM Fish f
        JOIN Users_Fish uf ON f.id = uf.fish_id
        LEFT JOIN Fish_Weather fw ON f.id = fw.fish_id
        LEFT JOIN Weathers w ON fw.weather_id = w.id
        LEFT JOIN Fish_Season fs ON f.id = fs.fish_id
        LEFT JOIN Seasons s ON fs.season_id = s.id
        LEFT JOIN Fish_FishLocation fl ON f.id = fl.fish_id
        LEFT JOIN FishLocations l ON fl.location_id = l.id
        WHERE uf.user_id = ?
        AND LOWER(s.name) LIKE '%' || LOWER(?)
        AND LOWER(w.name) LIKE '%' || LOWER(?)
        AND LOWER(l.name) LIKE '%' || LOWER(?)
        GROUP BY f.id
    )";

	// Prepare the SQL statement
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return filteredFish;
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, userId);
	sqlite3_bind_text(statement, 2, season.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 3, weather.c_str(), -1, SQLITE_STATIC);
	sqlite3_bind_text(statement, 4, location.c_str(), -1, SQLITE_STATIC);

	// Execute query and retrieve results
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);

		filteredFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return filteredFish;
}



/*
	Function that returns all the Fish objects from the database that have been caught by a specific user with the given username.
	Params:
		userId - the id of the logged user
*/
vector<Fish> FishDBRepository::findAllUncaught(const long userId) const noexcept {
	vector<Fish> uncaughtFish;
	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return uncaughtFish;
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return uncaughtFish;
	}

	// Prepare SQL query
	string query = R"(
        SELECT f.id, f.name, f.category, f.description, f.start_catching_hour, f.end_catching_hour, f.difficulty, f.movement, f.image 
		FROM Fish f
		JOIN Users_Fish uf ON f.id = uf.fish_id
		JOIN Fish_Weather fw ON f.id = fw.fish_id
		JOIN Weathers w ON fw.weather_id = w.id
		JOIN Fish_Season fs ON f.id = fs.fish_id
		JOIN Seasons s ON fs.season_id = s.id
		JOIN Fish_FishLocation fl ON f.id = fl.fish_id
		JOIN FishLocations l ON fl.location_id = l.id
		WHERE uf.user_id = ? AND uf.is_caught = 0
		GROUP BY f.id
    )";

	// Prepare the SQL statement
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return uncaughtFish;
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, userId);

	// Execute query and retrieve results
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);

		uncaughtFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return uncaughtFish;
}



/*
	Function that returns all the Fish objects from the database that have been marked as favorite by a specific user with the given username.
	Params:
		userId - the id of the logged user
*/
vector<Fish> FishDBRepository::findAllFavorite(const long userId) const noexcept {
	vector<Fish> favoriteFish;
	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return favoriteFish;
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return favoriteFish;
	}

	// Prepare SQL query
	string query = R"(
        SELECT f.id, f.name, f.category, f.description, f.start_catching_hour, f.end_catching_hour, f.difficulty, f.movement, f.image 
		FROM Fish f
		JOIN Users_Fish uf ON f.id = uf.fish_id
		JOIN Fish_Weather fw ON f.id = fw.fish_id
		JOIN Weathers w ON fw.weather_id = w.id
		JOIN Fish_Season fs ON f.id = fs.fish_id
		JOIN Seasons s ON fs.season_id = s.id
		JOIN Fish_FishLocation fl ON f.id = fl.fish_id
		JOIN FishLocations l ON fl.location_id = l.id
		WHERE uf.user_id = ? AND uf.is_favorite = 1
		GROUP BY f.id
    )";

	// Prepare the SQL statement
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return favoriteFish;
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, userId);

	// Execute query and retrieve results
	while ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int id = sqlite3_column_int(statement, 0);
		string name = reinterpret_cast<const char*>(sqlite3_column_text(statement, 1));
		string category = reinterpret_cast<const char*>(sqlite3_column_text(statement, 2));
		string description = reinterpret_cast<const char*>(sqlite3_column_text(statement, 3));
		string startCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 4));
		string endCatchingHour = reinterpret_cast<const char*>(sqlite3_column_text(statement, 5));
		long difficulty = sqlite3_column_int(statement, 6);
		string movement = reinterpret_cast<const char*>(sqlite3_column_text(statement, 7));
		const void* imageBlob = sqlite3_column_blob(statement, 8);
		int imageSize = sqlite3_column_bytes(statement, 8);
		std::vector<char> image(reinterpret_cast<const char*>(imageBlob), reinterpret_cast<const char*>(imageBlob) + imageSize);

		vector<string> seasons = getSeasonsByFishId(db, id);
		vector<string> weathers = getWeathersByFishId(db, id);
		vector<string> locations = getLocationsByFishId(db, id);
		long isCaught = getIsCaughtByFishId(db, id, userId);
		long isFavorite = getIsFavoriteByFishId(db, id, userId);

		favoriteFish.push_back(Fish(id, name, category, description, seasons, weathers, locations, startCatchingHour, endCatchingHour, difficulty, movement, isCaught, isFavorite, image));
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return favoriteFish;
}



/*
	Function that returns the number of fish that have been caught by a specific user.
	Params:
		userId - the id of the logged user
*/
const long FishDBRepository::getCaughtFishNumber(const long userId) const noexcept {
	long caughtFishNumber = 0;
	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return 0;
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return 0;
	}

	// Prepare SQL query
	string query = R"(
        SELECT COUNT(uf.fish_id) FROM Users_Fish uf
		WHERE uf.user_id = ? AND uf.is_caught = 1
    )";

	// Prepare the SQL statement
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return 0;
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, userId);

	// Execute query and retrieve results
	if ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int fishNo = sqlite3_column_int(statement, 0);
		caughtFishNumber = fishNo;
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return caughtFishNumber;
}



/*
	Function that returns the number of fish that have been marked as favorite by a specific user.
	Params:
		userId - the id of the logged user
*/
const long FishDBRepository::getFavoriteFishNumber(const long userId) const noexcept {
	long favoriteFishNumber = 0;
	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return 0;
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return 0;
	}

	// Prepare SQL query
	string query = R"(
        SELECT COUNT(uf.fish_id) FROM Users_Fish uf
		WHERE uf.user_id = ? AND uf.is_favorite = 1
    )";

	// Prepare the SQL statement
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return 0;
	}

	// Bind parameters
	sqlite3_bind_int(statement, 1, userId);

	// Execute query and retrieve results
	if ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		int fishNo = sqlite3_column_int(statement, 0);
		favoriteFishNumber = fishNo;
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return favoriteFishNumber;
}



/*
	Function that returns the number of registered fish in the database.
*/
const long FishDBRepository::findAllFishNumber() const noexcept {
	long fishNo = 0;
	sqlite3* db;
	sqlite3_stmt* statement;
	int rc;

	// Open connection to the database
	rc = sqlite3_open(databasePath.c_str(), &db);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to open database: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return 0;
	}

	// Begin transaction
	rc = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
	if (rc != SQLITE_OK) {
		qDebug() << "Failed to begin transaction: " << sqlite3_errmsg(db);
		sqlite3_close(db);
		return 0;
	}

	// Prepare SQL query
	string query = R"(
        SELECT COUNT(f.id) FROM Fish f
    )";

	// Prepare the SQL statement
	rc = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, nullptr);
	if (rc != SQLITE_OK) {
		std::cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
		sqlite3_finalize(statement);
		sqlite3_close(db);
		return 0;
	}

	// Execute query and retrieve results
	if ((rc = sqlite3_step(statement)) == SQLITE_ROW) {
		fishNo = sqlite3_column_int(statement, 0);
	}

	// Finalize statement and close connection
	sqlite3_finalize(statement);
	sqlite3_close(db);

	return fishNo;
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
			if (season)
				seasons.push_back(season);
		}

		sqlite3_finalize(statement);
	}
	else {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
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
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
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
			}
		}

		sqlite3_finalize(statement);
	}
	else {
		cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
	}
	return locations;
}



/*
	Helper function that returns whether a Fish object with the given id is caught by the user with the given username.
	Params:
		db - the database connection
		fishId - the id of the fish
		userId - the id of the user
*/
bool FishDBRepository::getIsCaughtByFishId(sqlite3* db, long fishId, const long userId) const {
	bool isCaught = false;
	sqlite3_stmt* statement;

	const char* caughtQuery = "SELECT is_caught FROM Users_Fish WHERE fish_id = ? AND user_id = ?";
	int rc = sqlite3_prepare_v2(db, caughtQuery, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, fishId);
		sqlite3_bind_int(statement, 2, userId);
		rc = sqlite3_step(statement);

		if (rc == SQLITE_ROW) {
			isCaught = sqlite3_column_int(statement, 0);
		}
	}

	sqlite3_finalize(statement);
	return isCaught;
}



/*
	Helper function that returns whether a Fish object with the given id is caught by the user with the given username.
	Params:
		db - the database connection
		fishId - the id of the fish
		userId - the id of the user
*/
bool FishDBRepository::getIsFavoriteByFishId(sqlite3* db, long fishId, const long userId) const {
	bool isCaught = false;
	sqlite3_stmt* statement;

	const char* caughtQuery = "SELECT is_favorite FROM Users_Fish WHERE fish_id = ? AND user_id = ?";
	int rc = sqlite3_prepare_v2(db, caughtQuery, -1, &statement, nullptr);
	if (rc == SQLITE_OK) {
		sqlite3_bind_int(statement, 1, fishId);
		sqlite3_bind_int(statement, 2, userId);
		rc = sqlite3_step(statement);

		if (rc == SQLITE_ROW) {
			isCaught = sqlite3_column_int(statement, 0);
		}
	}

	sqlite3_finalize(statement);
	return isCaught;
}



/*
	Function that modifies tables from the database
*/
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