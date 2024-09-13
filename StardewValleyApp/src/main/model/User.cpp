#include "User.h"
#include <string>
#include <sstream>

using namespace std;

User::User() {}
User::User(const long id, const string& name, const QPixmap& image) : Entity(id), name(name), image(image) {}

const long User::getId() const {
	return Entity::getId();
}

const string& User::getName() const {
    return this->name;
}

const QPixmap& User::getImage() const {
    return this->image;
}

void User::setName(const string& name) {
    this->name = name;
}

void User::setImage(const QPixmap& image) {
    this->image = image;
}

const string User::toString() const {
    std::ostringstream oss;
    oss << "User: ";
    oss << "Id: " << getId() << " ";
    oss << "Name: " << name << " ";
    oss << "Has Image: " << (image.size().height() > 0 ? "Yes" : "No");
    return oss.str();
}
