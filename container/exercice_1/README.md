After analysing the results of benchmarks of different containers, think about
what operations you use most of the time and try to implement a container that
better meets these needs.

Rules:
  - No rules when performance is the keyword.

Tips:
  - Don't hesitate to use malloc/realloc/free/memcpy/memset, it's generally
    discouraged by C++ developers but that allow to gain performance.