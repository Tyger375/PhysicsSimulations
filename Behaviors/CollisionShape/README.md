# CollisionShape

Used for adding collisions to an object

## How does it work

To detect collisions, I am using aabb algorithm

![img.png](Images/aabb_algorithm.png)

AABB stands for Axis-Aligned Bounding Box<br>
If all of these conditions are true, two object are colliding
```
other.left <= object.right
other.right >= object.left
other.top >= object.bottom
other.bottom <= object.top
```