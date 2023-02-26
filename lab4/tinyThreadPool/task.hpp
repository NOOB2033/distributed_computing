#ifndef DISTRIBUTED_COMPUTING_TASK_HPP
#define DISTRIBUTED_COMPUTING_TASK_HPP


#include <functional>

namespace tp {

using Task = std::function<void()>;

}  // namespace tp


#endif //DISTRIBUTED_COMPUTING_TASK_HPP
