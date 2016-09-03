#pragma once

#include <job/WorkContext.hpp>
#include <platform/FileIndex.hpp>

/**
 * Implementation of a worker that loads a resource in the background.
 */
template <class T, class L>
class BackgroundLoaderJob : public WorkJob
{
public:

  BackgroundLoaderJob(WorkContext* context, FileIndex* index, const std::string& file,
                      T*& ref)
      : WorkJob(context), index(index), filename(file), resourceRef(ref)
  {
  }

  void work()
  {
    data = index->openFile(filename);
  }

  void complete()
  {
    if (data) {
      L loader;

      resourceRef = loader.loadFromMemory(data);
    }
  }

private:
  FileIndex* index;
  std::string filename;
  FileHandle data;
  T*& resourceRef;
};
