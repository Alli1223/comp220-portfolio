import pybullet as p
p.connect(p.SHARED_MEMORY)
objects = [p.loadURDF("kuka_iiwa/model.urdf", 0.000000,0.000000,0.000000,0.000000,0.000000,0.000000,1.000000)]
ob = objects[0]
jointPositions=[ 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000 ]
for jointIndex in range (p.getNumJoints(ob)):
	p.resetJointState(ob,jointIndex,jointPositions[jointIndex])

p.setGravity(0.000000,0.000000,0.000000)
p.stepSimulation()
p.disconnect()
