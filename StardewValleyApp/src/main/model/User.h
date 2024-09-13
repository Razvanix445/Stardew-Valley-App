#include "Entity.h"
#include <QPixmap>
#include <qDebug>
#include <string>
#include <vector>

using namespace std;

class User : public Entity {

protected:
    string name;
    QPixmap image;

public:
    // Constructor
    User();
    User(const long id, const string& name, const QPixmap& image);

    // Get the User id
    const long getId() const;

    // Get the User name
    const string& getName() const;

    // Get the User image
    const QPixmap& getImage() const;

    // Set the User name to a new value
    void setName(const string& name);

    // Set the User image to a new value
    void setImage(const QPixmap& image);

    // Convert the User object to a string
    const string toString() const;
};