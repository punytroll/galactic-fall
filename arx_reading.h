#ifndef ARX_READING_H
#define ARX_READING_H

class Archive;
class Commodity;
class CommodityManager;
class Item;
class Label;
class Model;
class ModelManager;
class ShipClass;
class ShipClassManager;
class System;
class SystemManager;
class Widget;

Arxx::Item * GetItem(Arxx::Archive & Archive, Arxx::u4byte UniqueIdentifier);

Commodity * ReadCommodity(CommodityManager * CommodityManager, Arxx::Item * Item);
Label * ReadLabel(Arxx::Item * Item);
Model * ReadModel(ModelManager * ModelManager, Arxx::Item * Item);
ShipClass * ReadShipClass(ShipClassManager * ShipClassManager, Arxx::Item * Item);
System * ReadSystem(SystemManager * SystemManager, Arxx::Item * Item);
void ReadSystemLink(SystemManager * SystemManager, Arxx::Item * Item);
Widget * ReadWidget(Arxx::Item * Item, Widget * Widget = 0);

#endif
