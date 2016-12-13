#ifndef __POSITION_FILE_H__
#define __POSITION_FILE_H__

#include <string>
#include <vector>
#include <memory>
#include <fstream>

struct Position
{
  int objectId;
  int frameNum;
  int leftTopx;
  int leftTopy;
  int width;
  int height;
};

class PositionFile
{
 public:
  PositionFile(std::string fileName);
  ~PositionFile();
  int save();
  int load();
  std::shared_ptr<Position> getNext();
 private:
  std::vector<std::shared_ptr<Position>>  _postions;
  std::string _fileName;
};

#endif
