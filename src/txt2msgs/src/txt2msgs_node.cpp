#include <ros/ros.h>
#include <std_msgs/String.h>
#include <sstream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>  
#include <iostream>  
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include "txt2msgs/Pose_msgs.h"
#include <math.h>
#include <time.h>

using namespace std;

static sensor_msgs::PointCloud2 point_msg;
static ros::Publisher point_pub;
static txt2msgs::Pose_msgs ws_msg;
static ros::Publisher ws_pub;
static txt2msgs::Pose_msgs wv_msg;
static ros::Publisher wv_pub;

int main(int argc, char **argv)
{
	
	ros::init(argc, argv, "txt2msg_node");
	ros::NodeHandle nh;
 	point_pub = nh.advertise<sensor_msgs::PointCloud2>("/points_raw", 1000);
    ws_pub = nh.advertise<txt2msgs::Pose_msgs>("/ws_pose", 1000);
	wv_pub = nh.advertise<txt2msgs::Pose_msgs>("/wv_pose", 1000);

	ros::Rate ros_rate(10);
	
	for (int t = 1; t <= 2280; t++)
	{
		float times1=clock();
		char pcd_filename[100];
		sprintf(pcd_filename, "/home/yuzhanhai/software/points-map/point-pcd/point%d.pcd", t);
		
		pcl::PointCloud<pcl::PointXYZI>::Ptr inputcloud(new pcl::PointCloud<pcl::PointXYZI>());

		if (pcl::io::loadPCDFile<pcl::PointXYZI>(pcd_filename, *inputcloud) == -1)
		{
			cout << "error" << endl;
			return -1;
		}
	    
		char time_filename[100];
		sprintf(time_filename, "/home/yuzhanhai/software/points-map/time_laser/time_laser%d.txt",t);
		FILE *fp_time;
		double time, time_laser;
		fp_time = fopen(time_filename, "r");
        while (1 ==  fscanf(fp_time, "%lf\n", &time))
		{
			time_laser = time;
			//cout<<time_laser<<endl;
			//cout<<setiosflags(ios::fixed)<<time_laser<<endl;
		}
		fclose(fp_time);

		char ws_filename[100];
		sprintf(ws_filename, "/home/yuzhanhai/software/points-map/X_ws/X_ws%d.txt",t);
		FILE *fp_ws;
		float ws1,ws2,ws3,ws4,ws5,ws6;
		fp_ws=fopen(ws_filename, "r");
		while (6 == fscanf(fp_ws, "%f\t%f\t%f\t%f\t%f\t%f\t", &ws1,&ws2,&ws3,&ws4,&ws5,&ws6));
		{
			ws_msg.x=ws1;
			ws_msg.y=ws2;
			ws_msg.z=ws3;
			ws_msg.roll=ws4;
			ws_msg.pitch=ws5;
			ws_msg.yaw=ws6;
		}
		fclose(fp_ws);

		char wv_filename[100];
		sprintf(wv_filename, "/home/yuzhanhai/software/points-map/X_wv/X_wv%d.txt",t);
		FILE *fp_wv;
		float wv1,wv2,wv3,wv4,wv5,wv6;
		fp_wv=fopen(wv_filename, "r");
		while (6 == fscanf(fp_wv, "%f\t%f\t%f\t%f\t%f\t%f\t", &wv1,&wv2,&wv3,&wv4,&wv5,&wv6));
		{
			wv_msg.x=wv1;
			wv_msg.y=wv2;
			wv_msg.z=wv3;
			wv_msg.roll=wv4;
			wv_msg.pitch=wv5;
			wv_msg.yaw=wv6;
		}
		fclose(fp_wv);

		//pcl_conversions::toPCL(ros::Time::now(), inputcloud->header.stamp);
		
		time_laser=time_laser/1000000;
		//cout<<time_laser<<endl;
		uint64_t time_sec=floor(time_laser);
		uint64_t time_nsec=(time_laser-time_sec)*1000000000;
		
		//cout<<time_sec<<endl<<time_nsec<<endl;
		
        //inputcloud->header.stamp.sec =time_sec;
        //inputcloud->header.stamp.nsec=time_nsec;
   		//cout<<inputcloud->header.stamp<<endl;
        
        
        //inputcloud->header.stamp = time_laser;
		pcl::toROSMsg (*inputcloud, point_msg); 
		point_msg.header.stamp.sec =time_sec;
		point_msg.header.stamp.nsec=time_nsec;
		point_msg.header.frame_id = "velodyne";
		point_pub.publish(point_msg);
		cout<<point_msg.header.stamp<<endl;

		ws_msg.header.stamp.sec=time_sec;
		ws_msg.header.stamp.nsec=time_nsec;
		ws_msg.header.frame_id="velodyne";
		ws_pub.publish(ws_msg);

		wv_msg.header.stamp.sec=time_sec;
		wv_msg.header.stamp.nsec=time_nsec;
		wv_msg.header.frame_id="velodyne";
		wv_pub.publish(wv_msg);

		ros::spinOnce();
		ros_rate.sleep();
		float times2=clock();
		//cout<<"times1="<<times1<<endl;
		//cout<<"times2="<<times2<<endl;
		//cout<<(times2-times1)<<endl;
		float duration=(times2-times1)/1000000;
		std::cout<<t<<"\t"<<duration<<std::endl;
		//system("pause");
	}
		
	return 0;
}


