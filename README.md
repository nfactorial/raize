raize
=====

C++ Multi-threaded Task Scheduler

Raize is a part of my 3d graphics engine nGen, however I wanted it to be a standalone library for usage in other projects.

Core Goals
==========
Raize is intended to be simple to understand and integrate into any codebase, making use of of the standard library for things such as threading and synchronization.

The memory footprint is intended to be small, controllable and static. That is to say, dynamic memory allocation will be kept to a minimum as much as possible. This is to aid in its use within a game engine, which generally does not wish to have systems throwing memory around.

Whilst the main goal is for a multi-threaded task scheduler for games, it is also intended to be useful for other applications. However, you may consider the Intel Threaded Building Blocks (https://www.threadingbuildingblocks.org/) for a more general purpose threading implementation.

Workplan
========
  * Stage 1 - Multiple threads up and running -- <Current Stage>
  * Stage 2 - Add ability to process tasks on each thread
  * Stage 3 - Compiling for (at least) Linux and Windows, it should be compatible with other platforms also.
  * Stage 4 - Add dependency management, raize will automatically ensure dependencies do not conflict running tasks.
  * Stage 5 - Profiling support, raize will (optionally) maintain accessible performance information for all threads and tasks.

When stages 1 and 2 are complete, raize will not be in a very usable state. It wlil execute tasks but it wont take into account their data dependencies. It'll be great if you don't have any dependencies, but that's not that likely when you want to do anything meaningful.

Once raize is in a state that will actually do something, stage 3 will be taken to ensure it works on Linux and Windows machines, cross machine compatibility wlil be an ongoing task but the idea is to ensure it compiles with any C++ compiler.

Stage 4 will attempt to rectify this, with tasks able to specify data dependencies. Raize will then ensure no two tasks will run concurrently that conflict on their dependencies.

With all that done, it is important to be able to visualize how the threads are performing and where your bottlenecks are. Raize will, once stage 5 is complete, maintain performance information about all the tasks it processed. This information will be provided in a simple to navigate data chart, allowing the host application to display live profiling information. The host application will also be able to disable profile recording in a master release if desired.

Current Stage 1
===============
The core thread creation and execution code is written and tested with the first unit test (startup_shutdown.cpp), tested compiled on Microsoft Visual Studio 2013 Community Edition (http://www.visualstudio.com/en-us/news/vs2013-community-vs.aspx).

Will test compilation on OSX shortly and fix any issues. There are still some tweaks and tidying up to do before I consider Stage 1 complete.
