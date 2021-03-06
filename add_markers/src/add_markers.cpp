#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include "nav_msgs/Odometry.h"
#include <math.h>
// %EndTag(INCLUDES)%

//pickup coordinates
float pickupx = -4.88;
float pickupy = 3.60;
//dropoff coordinates
float dropoffx = 7.51;
float dropoffy = 5.98;

//pickup threshold
float threshold = 1.0f;

bool pickup = false;
bool dropoff = false;


void callback_odom(const nav_msgs::Odometry::ConstPtr &msg){

	float pose_x = msg->pose.pose.position.x;
	float pose_y = msg->pose.pose.position.y;
	float distance = 0.0;

	if(!pickup && !dropoff){
		//Robot needs instruction to go to the pickup location
		distance = sqrt(pow((pickupx - pose_x), 2) + pow((pickupy - pose_y), 2));
    		ROS_INFO("Distance to pickup = %f", distance);
    		if(distance <= threshold){
      			ROS_INFO("Robot in pick up zone");
      			pickup = true;
		}
	} 
        if(pickup && !dropoff){
		//Robot needs instruction to go to the dropoff location
		distance = sqrt(pow((dropoffx - pose_x), 2) + pow((dropoffy - pose_y), 2));
    		ROS_INFO("Distance to dropoff = %f", distance);
    		if(distance <= threshold){
      			ROS_INFO("Robot in drop off zone");
			pickup = false;
      			dropoff = true;
		}
	}

}

// %Tag(INIT)%
int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odom_subscriber = n.subscribe("/odom", 500, callback_odom);
// %EndTag(INIT)%

  // Set our initial shape type to be a cube
// %Tag(SHAPE_INIT)%
  uint32_t shape = visualization_msgs::Marker::CUBE;
// %EndTag(SHAPE_INIT)%

// %Tag(MARKER_INIT)%
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();
// %EndTag(MARKER_INIT)%

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
// %Tag(NS_ID)%
    marker.ns = "box";
    marker.id = 0;
// %EndTag(NS_ID)%

    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
// %Tag(TYPE)%
    marker.type = shape;
// %EndTag(TYPE)%

    // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
// %Tag(ACTION)%
    marker.action = visualization_msgs::Marker::ADD;
// %EndTag(ACTION)%

    // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
// %Tag(POSE)%
    marker.pose.position.x = pickupx;
    marker.pose.position.y = pickupy;
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
// %EndTag(POSE)%

    // Set the scale of the marker -- 1x1x1 here means 1m on a side
// %Tag(SCALE)%
    marker.scale.x = 0.3;
    marker.scale.y = 0.3;
    marker.scale.z = 0.3;
// %EndTag(SCALE)%

    // Set the color -- be sure to set alpha to something non-zero!
// %Tag(COLOR)%
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;
// %EndTag(COLOR)%

// %Tag(LIFETIME)%
    marker.lifetime = ros::Duration();
// %EndTag(LIFETIME)%

while (ros::ok() && !(pickup && dropoff)){

    // Publish the marker
// %Tag(PUBLISH)%
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }

   if(pickup){
	marker.action = visualization_msgs::Marker::DELETE;
      	ros::Duration(5.0).sleep();
	ROS_INFO("Object picked up!");
   }
   if(dropoff){
	marker.pose.position.x = dropoffx;
      	marker.pose.position.y = dropoffy;
      	marker.action = visualization_msgs::Marker::ADD;
      	ros::Duration(5.0).sleep();
	ROS_INFO("Object dropped off!");
	pickup = true;

   }

    marker_pub.publish(marker);
    ros::spinOnce();
// %EndTag(PUBLISH)%

  }
// %EndTag(SLEEP_END)%
}
