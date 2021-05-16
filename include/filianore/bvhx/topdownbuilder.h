#ifndef _TOP_DOWN_BUILDER_H
#define _TOP_DOWN_BUILDER_H

#include <cassert>
#include <stack>
#include <functional>
#include <iostream>
#include <optional>

namespace filianore
{

    // Base class for Top-Down Build tasks
    class TopDownBuildTask
    {
    public:
        struct WorkItem
        {
            size_t nodeIndex;
            size_t begin;
            size_t end;
            size_t depth;

            WorkItem() = default;
            WorkItem(size_t _nodeIndex, size_t _begin, size_t _end, size_t _depth)
                : nodeIndex(_nodeIndex), begin(_begin), end(_end), depth(_depth)
            {
            }

            size_t WorkSize() const { return end - begin; }
        };

        std::optional<std::pair<WorkItem, WorkItem>> Build(const WorkItem &workItem)
        {
            return std::nullopt;
        }
    };

    // Base class for Top-Down BVH Builders
    class TopDownBuilder
    {
    public:
        size_t taskSpawnThreshold = 1024;
        size_t maxDepth = 64;
        size_t maxLeafSize = 16;

        ~TopDownBuilder() {}

        template <typename... Args>
        void RunTask(TopDownBuildTask &task, Args &&...args)
        {
            using WorkItem = TopDownBuildTask::WorkItem;

            std::stack<WorkItem> workToDo;
            workToDo.emplace(std::forward<Args &&>(args)...);

            while (!workToDo.empty())
            {
                WorkItem workItem = workToDo.top();

                assert(workItem.depth <= maxDepth);
                workToDo.pop();

                std::optional<std::pair<WorkItem, WorkItem>> moreWork = task.Build(workItem);
                if (moreWork)
                {
                    if (moreWork->first.WorkSize() > moreWork->second.WorkSize())
                    {
                        std::swap(moreWork->first, moreWork->second);
                    }

                    workToDo.push(moreWork->second);

                    WorkItem firstItem = moreWork->first;
                    if (firstItem.WorkSize() > taskSpawnThreshold)
                    {
                        TopDownBuildTask newTask(task);
#pragma omp task firstprivate(newTask, firstItem)
                        {
                            RunTask(newTask, firstItem);
                        }
                    }
                    else
                    {
                        workToDo.push(firstItem);
                    }
                }
            }
        }
    };
}

#endif