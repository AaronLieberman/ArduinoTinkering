import bpy
import os
from pathlib import Path

arma = bpy.data.objects['Armature']

filePath = Path("D:\\Projects\\Git\\ArduinoTinkering\\RobotAnimations\\animations.txt")
if filePath.is_dir():
	os.remove(filePath)

with filePath.open("w") as outFile:

	for act in bpy.data.actions:
		if act.name == 'ArmatureAction':
			continue

		outFile.write("Action {name}\n"
			.format(name = act.name))

		arma.animation_data.action = act
		rotations = {}

		for frameIndex in range(0, int(act.frame_range.y + 1)):
			bpy.context.scene.frame_set(frameIndex)
			outFile.write("  Frame {index}\n"
				.format(index = frameIndex))
	
			for pbone in arma.pose.bones:
				angle = degrees(pbone.rotation_quaternion.angle)
				
				if rotations.get(pbone.name, '') != angle:
					outFile.write("    Bone {name}: {rot}\n"
						.format(name = pbone.name, rot = int(angle)))
					rotations[pbone.name] = angle

			outFile.write("  EndFrame\n")
		outFile.write("EndAction\n")
