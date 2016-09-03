#include <loaders/LoaderIDE.hpp>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <map>
#include <sstream>
#include <string>

bool LoaderIDE::load(const std::string &filename)
{
  std::ifstream str(filename);

  if (!str.is_open()) return false;

  SectionTypes section = NONE;
  while (!str.eof()) {
    std::string line;
    getline(str, line);
    line.erase(std::find_if(line.rbegin(), line.rend(),
                            std::not1(std::ptr_fun<int, int>(std::isspace)))
                   .base(),
               line.end());

    if (!line.empty() && line[0] == '#') continue;

    if (line == "end") {
      section = NONE;
    } else if (section == NONE) {
      if (line == "objs") {
        section = OBJS;
      } else if (line == "tobj") {
        section = TOBJ;
      } else if (line == "peds") {
        section = PEDS;
      } else if (line == "cars") {
        section = CARS;
      } else if (line == "hier") {
        section = HIER;
      } else if (line == "2dfx") {
        section = TWODFX;
      } else if (line == "path") {
        section = PATH;
      }
    } else {
      // Remove ALL the whitespace!!
      line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

      std::stringstream strstream(line);

      switch (section) {
        default:
          break;
        case OBJS:
        case TOBJ: {  // Supports Type 1, 2 and 3
          std::shared_ptr<SimpleModelData> objs(new SimpleModelData);

          std::string id, numClumps, flags, modelName, textureName;

          // Read the content of the line
          getline(strstream, id, ',');
          getline(strstream, modelName, ',');
          getline(strstream, textureName, ',');
          getline(strstream, numClumps, ',');

          objs->numClumps = atoi(numClumps.c_str());
          for (size_t i = 0; i < objs->numClumps; i++) {
            std::string drawDistance;
            getline(strstream, drawDistance, ',');
            objs->drawDistance[i] = atoi(drawDistance.c_str());
          }

          getline(strstream, flags, ',');

          // Keep reading TOBJ data
          if (section == LoaderIDE::TOBJ) {
            std::string buff;
            getline(strstream, buff, ',');
            objs->timeOn = atoi(buff.c_str());
            getline(strstream, buff, ',');
            objs->timeOff = atoi(buff.c_str());
          } else {
            objs->timeOn = 0;
            objs->timeOff = 24;
          }

          // Put stuff in our struct
          objs->id = atoi(id.c_str());
          objs->flags = atoi(flags.c_str());
          objs->modelName = modelName;
          objs->textureName = textureName;
          objs->LOD = false;

          if (modelName.find("LOD", 0, 3) != modelName.npos &&
              modelName != "LODistancoast01") {
            objs->LOD = true;
          }

          objects.insert({objs->id, objs});
          break;
        }
        case CARS: {
          std::shared_ptr<VehicleModelData> cars(new VehicleModelData);

          std::string id, type, classType, frequency, lvl, comprules, wheelModelID,
              wheelScale;

          getline(strstream, id, ',');
          getline(strstream, cars->modelName, ',');
          getline(strstream, cars->textureName, ',');
          getline(strstream, type, ',');
          getline(strstream, cars->handlingID, ',');
          getline(strstream, cars->gameName, ',');
          getline(strstream, classType, ',');
          getline(strstream, frequency, ',');
          getline(strstream, lvl, ',');
          getline(strstream, comprules, ',');
          getline(strstream, wheelModelID, ',');
          getline(strstream, wheelScale, ',');

          cars->id = atoi(id.c_str());
          cars->frequency = atoi(frequency.c_str());
          cars->lvl = atoi(lvl.c_str());
          cars->comprules = atoi(comprules.c_str());

          if (type == "car") {
            cars->vehicletype = VehicleModelData::CAR;
            cars->wheelModelID = atoi(wheelModelID.c_str());
            cars->wheelScale = atof(wheelScale.c_str());
          } else if (type == "boat") {
            cars->vehicletype = VehicleModelData::BOAT;
          } else if (type == "train") {
            cars->vehicletype = VehicleModelData::TRAIN;
            cars->modelLOD = atoi(wheelModelID.c_str());
          } else if (type == "plane") {
            cars->vehicletype = VehicleModelData::PLANE;
          } else if (type == "heli") {
            cars->vehicletype = VehicleModelData::HELI;
          }

          const std::map<VehicleModelData::VehicleClass, std::string> classTypes{
              {VehicleModelData::IGNORE, "ignore"},
              {VehicleModelData::NORMAL, "normal"},
              {VehicleModelData::POORFAMILY, "poorfamily"},
              {VehicleModelData::RICHFAMILY, "richfamily"},
              {VehicleModelData::EXECUTIVE, "executive"},
              {VehicleModelData::WORKER, "worker"},
              {VehicleModelData::BIG, "big"},
              {VehicleModelData::TAXI, "taxi"},
              {VehicleModelData::MOPED, "moped"},
              {VehicleModelData::MOTORBIKE, "motorbike"},
              {VehicleModelData::LEISUREBOAT, "leisureboat"},
              {VehicleModelData::WORKERBOAT, "workerboat"},
              {VehicleModelData::BICYCLE, "bicycle"},
              {VehicleModelData::ONFOOT, "onfoot"},
          };
          for (auto &a : classTypes) {
            if (classType == a.second) {
              cars->classType = a.first;
              break;
            }
          }

          objects.insert({cars->id, cars});
          break;
        }
        case PEDS: {
          std::shared_ptr<CharacterModelData> peds(new CharacterModelData);

          std::string id, driveMask;

          getline(strstream, id, ',');
          getline(strstream, peds->modelName, ',');
          getline(strstream, peds->textureName, ',');
          getline(strstream, peds->type, ',');
          getline(strstream, peds->behaviour, ',');
          getline(strstream, peds->animGroup, ',');
          getline(strstream, driveMask, ',');

          peds->id = atoi(id.c_str());
          peds->driveMask = atoi(driveMask.c_str());

          objects.insert({peds->id, peds});
          break;
        }
        case PATH: {
          PathData path;

          std::string type;
          getline(strstream, type, ',');
          if (type == "ped") {
            path.type = PathData::PATH_PED;
          } else if (type == "car") {
            path.type = PathData::PATH_CAR;
          }

          std::string id;
          getline(strstream, id, ',');
          path.ID = atoi(id.c_str());

          getline(strstream, path.modelName);

          std::string linebuff, buff;
          for (size_t p = 0; p < 12; ++p) {
            PathNode node;

            getline(str, linebuff);
            std::stringstream buffstream(linebuff);

            getline(buffstream, buff, ',');
            switch (atoi(buff.c_str())) {
              case 0:
                node.type = PathNode::EMPTY;
                break;
              case 2:
                node.type = PathNode::INTERNAL;
                break;
              case 1:
                node.type = PathNode::EXTERNAL;
                break;
            }

            if (node.type == PathNode::EMPTY) {
              continue;
            }

            getline(buffstream, buff, ',');
            node.next = atoi(buff.c_str());

            getline(buffstream, buff, ',');  // "Always 0"

            getline(buffstream, buff, ',');
            node.position.x = atof(buff.c_str()) * 1 / 16.f;

            getline(buffstream, buff, ',');
            node.position.y = atof(buff.c_str()) * 1 / 16.f;

            getline(buffstream, buff, ',');
            node.position.z = atof(buff.c_str()) * 1 / 16.f;

            getline(buffstream, buff, ',');
            node.size = atof(buff.c_str()) * 1 / 16.f;

            getline(buffstream, buff, ',');
            node.other_thing = atoi(buff.c_str());

            getline(buffstream, buff, ',');
            node.other_thing2 = atoi(buff.c_str());

            path.nodes.push_back(node);
          }

          auto &object = objects[path.ID];
          auto instance = std::dynamic_pointer_cast<SimpleModelData>(object);
          instance->paths.push_back(path);

          break;
        }
        case HIER: {
          std::shared_ptr<CutsceneModelData> cut(new CutsceneModelData);

          std::string id;

          getline(strstream, id, ',');
          getline(strstream, cut->modelName, ',');
          getline(strstream, cut->textureName, ',');

          cut->id = atoi(id.c_str());

          objects.insert({cut->id, cut});
          break;
        }
      }
    }
  }

  return true;
}
