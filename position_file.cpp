
#include "position_file.h"

PositionFile::PositionFile(std::string fileName)
{
  _fileName = fileName;
}

PositionFile::~PositionFile()
{
  
}

int PositionFile::load()
{
  std::ifstream file(_fileName,std::ios::out);
  if( !file )
  {
    return -1;
  }

  std::string tmp;
  while( file.eof() )
  {
    file>>tmp;
  }

  file.close();
  return 0;
}

int PositionFile::save()
{
  return 0;
}


