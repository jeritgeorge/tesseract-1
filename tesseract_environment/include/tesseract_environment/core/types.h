/**
 * @file types.h
 * @brief The tesseract_environment package types.
 *
 * @author Levi Armstrong
 * @date April 15, 2018
 * @version TODO
 * @bug No known bugs
 *
 * @copyright Copyright (c) 2013, Southwest Research Institute
 *
 * @par License
 * Software License Agreement (Apache License)
 * @par
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * @par
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef TESSERACT_ENVIRONMENT_TYPES_H
#define TESSERACT_ENVIRONMENT_TYPES_H

#include <tesseract_environment/core/macros.h>
TESSERACT_ENVIRONMENT_IGNORE_WARNINGS_PUSH
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/StdVector>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>
#include <map>
TESSERACT_ENVIRONMENT_IGNORE_WARNINGS_POP

namespace tesseract_environment
{
template <typename T>
using AlignedVector = std::vector<T, Eigen::aligned_allocator<T>>;

template <typename Key, typename Value>
using AlignedMap = std::map<Key, Value, std::less<Key>, Eigen::aligned_allocator<std::pair<const Key, Value>>>;

template <typename Key, typename Value>
using AlignedUnorderedMap = std::unordered_map<Key,
                                               Value,
                                               std::hash<Key>,
                                               std::equal_to<Key>,
                                               Eigen::aligned_allocator<std::pair<const Key, Value>>>;

using VectorIsometry3d = AlignedVector<Eigen::Isometry3d>;
using VectorVector4d = AlignedVector<Eigen::Vector4d>;
using VectorVector3d = std::vector<Eigen::Vector3d>;
using TransformMap = AlignedMap<std::string, Eigen::Isometry3d>;

typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> TrajArray;

struct AllowedCollisionMatrix
{
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  virtual ~AllowedCollisionMatrix() = default;

  /**
   * @brief Disable collision between two collision objects
   * @param obj1 Collision object name
   * @param obj2 Collision object name
   * @param reason The reason for disabling collison
   */
  virtual void addAllowedCollision(const std::string& link_name1,
                                   const std::string& link_name2,
                                   const std::string& reason)
  {
    lookup_table_[link_name1 + link_name2] = reason;
    lookup_table_[link_name2 + link_name1] = reason;
  }

  /**
   * @brief Remove disabled collision pair from allowed collision matrix
   * @param obj1 Collision object name
   * @param obj2 Collision object name
   */
  virtual void removeAllowedCollision(const std::string& link_name1, const std::string& link_name2)
  {
    lookup_table_.erase(link_name1 + link_name2);
    lookup_table_.erase(link_name2 + link_name1);
  }

  /**
   * @brief This checks if two links are allowed to be in collision
   * @param link_name1 First link name
   * @param link_name2 Second link anme
   * @return True if allowed to be in collision, otherwise false
   */
  virtual bool isCollisionAllowed(const std::string& link_name1, const std::string& link_name2) const
  {
    return (lookup_table_.find(link_name1 + link_name2) != lookup_table_.end());
  }

private:
  std::unordered_map<std::string, std::string> lookup_table_;
};
typedef std::shared_ptr<AllowedCollisionMatrix> AllowedCollisionMatrixPtr;
typedef std::shared_ptr<const AllowedCollisionMatrix> AllowedCollisionMatrixConstPtr;

namespace BodyTypes
{
enum BodyType
{
  ROBOT_LINK = 0,    /**< @brief These are links at the creation of the environment */
  ROBOT_ATTACHED = 1 /**< @brief These are links that are added after initial creation */
};
}
typedef BodyTypes::BodyType BodyType;

/** @brief This holds a state of the environment */
struct EnvState
{
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

  std::unordered_map<std::string, double> joints;
  TransformMap transforms;
};
typedef std::shared_ptr<EnvState> EnvStatePtr;
typedef std::shared_ptr<const EnvState> EnvStateConstPtr;

///**< @brief Information on how the object is attached to the environment */
//struct AttachedBodyInfo
//{
//  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

//  AttachedBodyInfo() : transform(Eigen::Isometry3d::Identity()) {}
//  std::string object_name;              /**< @brief The name of the AttachableObject being used */
//  std::string parent_link_name;         /**< @brief The name of the link to attach the body */
//  Eigen::Isometry3d transform;          /**< @brief The transform between parent link and object */
//  std::vector<std::string> touch_links; /**< @brief The names of links which the attached body is allowed to be in
//                                           contact with */
//};

///** @brief Contains visual geometry data */
//struct VisualObjectGeometry
//{
//  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

//  std::vector<shapes::ShapeConstPtr> shapes; /**< @brief The shape */
//  VectorIsometry3d shape_poses;              /**< @brief The pose of the shape */
//  VectorVector4d shape_colors;               /**< @brief (Optional) The shape color (R, G, B, A) */
//};

///** @brief Contains visual geometry data */
//struct CollisionObjectGeometry
//{
//  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

//  tesseract_collision::CollisionShapesConst shapes; /**< @brief The collision shape */
//  VectorIsometry3d shape_poses;                     /**< @brief The pose of the shape */
//  VectorVector4d shape_colors;                      /**< @brief (Optional) The shape color (R, G, B, A) */
//};

///** @brief Contains data about an attachable object */
//struct AttachableObject
//{
//  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

//  std::string name;            /**< @brief The name of the attachable object (aka. link name and must be unique) */
//  VisualObjectGeometry visual; /**< @brief The objects visual geometry */
//  CollisionObjectGeometry collision; /**< @brief The objects collision geometry */
//};
//typedef std::shared_ptr<AttachableObject> AttachableObjectPtr;
//typedef std::shared_ptr<const AttachableObject> AttachableObjectConstPtr;

///** @brief ObjectColorMap Stores Object color in a 4d vector as RGBA*/
//struct ObjectColor
//{
//  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

//  VectorVector4d visual;
//  VectorVector4d collision;
//};
//typedef AlignedUnorderedMap<std::string, ObjectColor> ObjectColorMap;
//typedef std::shared_ptr<ObjectColorMap> ObjectColorMapPtr;
//typedef std::shared_ptr<const ObjectColorMap> ObjectColorMapConstPtr;
//typedef AlignedUnorderedMap<std::string, AttachedBodyInfo> AttachedBodyInfoMap;
//typedef std::unordered_map<std::string, AttachableObjectConstPtr> AttachableObjectConstPtrMap;
}

#endif  // TESSERACT_ENVIRONMENT_TYPES_H
