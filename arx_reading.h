#ifndef ARX_READING_H
#define ARX_READING_H

class Archive;
class Item;
class Label;
class Model;
class ModelManager;
class ShipClass;
class ShipClassManager;
class Widget;

Arxx::Item * GetItem(Arxx::Archive & Archive, Arxx::u4byte UniqueIdentifier);

Label * ReadLabel(Arxx::Item * Item);
Model * ReadModel(ModelManager * ModelManager, Arxx::Item * Item);
ShipClass * ReadShipClass(ShipClassManager * ShipClassManager, Arxx::Item * Item);
Widget * ReadWidget(Arxx::Item * Item);

#endif
