#ifndef ARX_READING_H
#define ARX_READING_H

class Archive;
class CommodityManager;
class Item;
class ModelManager;
class ShipClassManager;
class SystemManager;
class UserInterface;

Arxx::Item * GetItem(Arxx::Archive & Archive, Arxx::u4byte UniqueIdentifier);

void ReadCommodities(Arxx::Archive & Archive, CommodityManager * Manager);
void ReadModels(Arxx::Archive & Archive, ModelManager * Manager);
void ReadShipClasses(Arxx::Archive & Archive, ShipClassManager * Manager);
void ReadSystems(Arxx::Archive & Archive, SystemManager * Manager);
void ReadSystemLinks(Arxx::Archive & Archive, SystemManager * Manager);
void ReadUserInterface(Arxx::Archive & Archive, UserInterface * UserInterface);

#endif
