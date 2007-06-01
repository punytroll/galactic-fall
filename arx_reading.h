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

void ReadCommodities(Arxx::Archive & Archive, CommodityManager * Manager);
void ReadModels(Arxx::Archive & Archive, ModelManager * Manager);
void ReadShipClasses(Arxx::Archive & Archive, ShipClassManager * Manager);
void ReadSystems(Arxx::Archive & Archive, SystemManager * Manager);
void ReadSystemLinks(Arxx::Archive & Archive, SystemManager * Manager);

Label * ReadLabel(Arxx::Item * Item);
Widget * ReadWidget(Arxx::Item * Item, Widget * Widget = 0);

#endif
