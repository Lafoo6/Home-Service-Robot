#!/bin/sh


xterm  -e  " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(rospack find my_robot)/map/World.world " &
sleep 5
xterm  -e  " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(rospack find my_robot)/map/map.yaml" &
sleep 5
xterm -e " rosrun rviz rviz -d $(rospack find my_robot)/rvizConfig/rviz_navigation.rviz " &
sleep 5
xterm -e "rosrun add_markers add_markers " &
sleep 5
xterm -e "rosrun pick_objects pick_objects "
