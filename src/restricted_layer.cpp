#include<restricted_layer/restricted_layer.h>
#include <pluginlib/class_list_macros.h>  
  

  
using costmap_2d::LETHAL_OBSTACLE;  
  
namespace restricted_layer_namespace
{  
  
RestrictedLayer::RestrictedLayer() {}
  
void RestrictedLayer::onInitialize()
{  
  ros::NodeHandle nh("~/" + name_);  
  current_ = true;  
  
  dsrv_ = new dynamic_reconfigure::Server<costmap_2d::GenericPluginConfig>(nh);  
  dynamic_reconfigure::Server<costmap_2d::GenericPluginConfig>::CallbackType cb = boost::bind(  
      &RestrictedLayer::reconfigureCB, this, _1, _2);
  dsrv_->setCallback(cb);  
}  
  
  
void RestrictedLayer::reconfigureCB(costmap_2d::GenericPluginConfig &config, uint32_t level)
{  
  enabled_ = config.enabled;  
}  
  
void RestrictedLayer::updateBounds(double origin_x, double origin_y, double origin_yaw, double* min_x,
                                           double* min_y, double* max_x, double* max_y)  
{  
  if (!enabled_)  
    return;  
  
  mark_x_ = origin_x + cos(origin_yaw);  
  mark_y_ = origin_y + sin(origin_yaw);  
  
  *min_x = std::min(*min_x, mark_x_);  
  *min_y = std::min(*min_y, mark_y_);  
  *max_x = std::max(*max_x, mark_x_);  
  *max_y = std::max(*max_y, mark_y_);  
}  
  
void RestrictedLayer::updateCosts(costmap_2d::Costmap2D& master_grid, int min_i, int min_j, int max_i,
                                          int max_j)  
{  
  if (!enabled_)  
    return;  
  unsigned int mx;  
  unsigned int my;  
  mark_x_ = -1.4; mark_y_ = -2.65;
  if(master_grid.worldToMap(mark_x_, mark_y_, mx, my)){  
    for(int i=0;i<40;i++){
      for(int j=0;j<40;j++){
        master_grid.setCost(mx+i, my+j, LETHAL_OBSTACLE);
      }
    }
  }  
}  
  
} // end namespace  

PLUGINLIB_EXPORT_CLASS(restricted_layer_namespace::RestrictedLayer, costmap_2d::Layer)
