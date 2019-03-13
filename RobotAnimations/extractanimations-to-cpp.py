import bpy
import os
from pathlib import Path

arma = bpy.data.objects['Armature']

# std::vector<Action> animationActions = {
#   Action { "Dab", 
#     std::vector<Frame> {
#       Frame { std::vector<short> { 0, 13, 0, 0, 90, 13, 0, 90 } }
#     }
#   }
#   ...
# };

filePath = Path("D:\\Projects\\Git\\ArduinoTinkering\\RobotAnimations\\animationData.h")
if filePath.is_dir():
	os.remove(filePath)

with filePath.open("w") as outFile:
	outFile.write("#pragma once\n")
	outFile.write("\n")
	outFile.write("const std::vector<Action> kActions = {\n")

	for act in bpy.data.actions:
		if act.name == 'ArmatureAction':
			continue

		outFile.write('  Action {{ "{name}", \n'
			.format(name = act.name))
		outFile.write('    std::vector<Frame> {\n')

		arma.animation_data.action = act

		for frameIndex in range(0, int(act.frame_range.y + 1)):
			bpy.context.scene.frame_set(frameIndex)
			outFile.write('      Frame {{ {index}, {{ '
				.format(index = frameIndex))

			for pbone in arma.pose.bones:
				angle = degrees(pbone.rotation_quaternion.angle)
				
				outFile.write("{rot}, "
					.format(rot = int(angle)))

			outFile.write('} },\n')

		outFile.write("    }\n")
		outFile.write("  },\n")

	outFile.write("};")
