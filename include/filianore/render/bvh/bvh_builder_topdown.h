#ifndef _BVH_BUILDER_TOPDOWN_H
#define _BVH_BUILDER_TOPDOWN_H


#include <omp.h>
#include <stack>
#include <cassert>


namespace filianore
{

    class BvhBuilderTopDownTask
    {
    protected:
        struct WorkItem
        {
            size_t nodeIndex;
            size_t begin;
            size_t end;
            size_t depth;

            WorkItem() = default;
            WorkItem(size_t _nodeIndex, size_t _begin, size_t _end, size_t _depth)
                : nodeIndex(_nodeIndex), begin(_begin), end(_end), depth(_depth)
            { }

            size_t WorkSize() const
            {
                return end - begin;
            }
        };
    };


    class BvhBuilderTopDown
    {
    public:

        size_t taskSpawnThreshold = 1024;
        size_t maxDepth = 64;
        size_t maxLeafSize = 16;


    protected:

        ~BvhBuilderTopDown() {}

        template <typename BuildTask, typename... Args>
        void RunTask(BuildTask& task, Args&&... args)
        {
            using WorkItem = typename BuildTask::WorkItemType;
            std::stack<WorkItem> stack;

            stack.emplace(std::forward<Args&&>(args)...);

            while (!stack.empty())
            {
                auto workItem = stack.top();
                assert(workItem.depth <= maxDepth);
                stack.pop();

                auto moreWork = task.Build(workItem);
                if (moreWork)
                {
                    if (moreWork->first.WorkSize() > moreWork->second.WorkSize())
                    {
                        std::swap(moreWork->first, moreWork->second);
                    }

                    stack.push(moreWork->second);

                    auto firstItem = moreWork->first;
                    if (firstItem.WorkSize() > taskSpawnThreshold)
                    {
                        BuildTask newTask(task);

                        #pragma omp task firstprivate(newTask, firstItem)
                        {
                            RunTask(newTask, firstItem);
                        }
                    }
                    else
                    {
                        stack.push(firstItem);
                    }

                }
            }
        }

    };

}


#endif