#ifndef FISHDBREPOSITORY_H
#define FISHDBREPOSITORY_H

#include "IRepository.h"
#include "Fish.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class FishDBRepository : public IRepository<Fish> {
private:
    string databasePath;

public:
    FishDBRepository(const string& databasePath);
    Fish findOne(long id) const override;
    Fish findOneByName(string& name) const;
    vector<Fish> findAll() const override;
    void save(Fish& fish) override;
    void remove(long id) override;
    void update(const Fish& fish) override;
};

#endif // FISHDBREPOSITORY_H
