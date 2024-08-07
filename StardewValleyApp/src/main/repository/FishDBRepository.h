#ifndef FISHDBREPOSITORY_H
#define FISHDBREPOSITORY_H

#include "IRepository.h"
#include "../model/Fish.h"
#include "../../resources/sqlite/sqlite3.h"
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>

using namespace std;

class FishDBRepository : public IRepository<Fish> {
private:
    string databasePath;

public:

    FishDBRepository() = default;

    FishDBRepository(const FishDBRepository& other) = delete;



    FishDBRepository(const string& databasePath);

    Fish findOne(long id, const string& username) const override;

    Fish findOneByName(const string& name, const string& username) const;

    vector<Fish> findAll(const string& username) const override;

    void save(Fish& fish) override;

    void remove(long id) override;

    void update(const Fish& fish) override;

    void saveImage(long fishId, const std::vector<char>& image);

    std::vector<char> getImage(long fishId) const;


    vector<string> findAllWeathers() const noexcept;

    vector<string> findAllSeasons() const noexcept;

    vector<string> findAllLocations() const noexcept;

    vector<Fish> findAllByWeather(const string& username, const string& weather) const noexcept;

    vector<Fish> findAllBySeason(const string& username, const string& season) const noexcept;

    vector<Fish> findAllByLocation(const string& username, const string& location) const noexcept;


    vector<string> getSeasonsByFishId(sqlite3* db, long fishId) const;

    vector<string> getWeathersByFishId(sqlite3* db, long fishId) const;

    vector<string> getLocationsByFishId(sqlite3* db, long fishId) const;

    bool getIsCaughtByFishId(sqlite3* db, long fishId, const string& username) const;

    void updateRelatedTable(sqlite3* db, const std::string& fishName, const std::vector<std::string>& items, const std::string& tableName, const std::string& itemIdColumn);


};

#endif // FISHDBREPOSITORY_H