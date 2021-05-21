#ifndef _TOP_DOWN_BUILDER_H
#define _TOP_DOWN_BUILDER_H

#include <cassert>
#include <stack>
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

        virtual std::optional<std::pair<WorkItem, WorkItem>> Build(const WorkItem &workItem) = 0;
    };

    // Base class for Top-Down BVH Builders
    class TopDownBuilder
    {
    public:
        using WorkItem = TopDownBuildTask::WorkItem;
        ~TopDownBuilder() {}

        void RunTask(TopDownBuildTask &task, WorkItem workItemArgs)
        {
            std::stack<WorkItem> workToDo;
            workToDo.push(workItemArgs);

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
                        //TopDownBuildTask newTask = task;
                        //#pragma omp task firstprivate(newTask, firstItem)
                        {
                            RunTask(task, firstItem);
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