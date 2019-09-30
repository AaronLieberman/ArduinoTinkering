import bpy
import os
from pathlib import Path

arma = bpy.data.objects['Armature']

# std::vector<Action> animationActions = {
#   Action { "Dab", 1,
#     {
#       Frame { { 13, 0, 0, 90, 13, 0, 90 } }
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
	outFile.write("#include \"Action.h\"\n")
	outFile.write("\n")
	outFile.write("const int kActionCount = {actionCount};\n"
		.format(actionCount = len(bpy.data.actions)))
	outFile.write("const Action kActions[{actionCount}] = {{\n"
		.format(actionCount = len(bpy.data.actions)))

	for act in bpy.data.actions:
		if act.name == 'ArmatureAction':
			continue

		outFile.write('  Action {{ "{name}", {frameCount},\n'
			.format(name = act.name, frameCount = int(act.frame_range.y + 1)))
		outFile.write('    {\n')

		arma.animation_data.action = act

		for frameIndex in range(0, int(act.frame_range.y + 1)):
			bpy.context.scene.frame_set(frameIndex)
			outFile.write('      Frame {{ {index}, {{ '
				.format(index = frameIndex))

			for pbone in arma.pose.bones[1:]:
				angle = degrees(pbone.rotation_euler.x + pbone.rotation_euler.y + pbone.rotation_euler.z)
				
				outFile.write("{rot}, "
					.format(rot = int(angle)))

			outFile.write('} },\n')

		outFile.write("    }\n")
		outFile.write("  },\n")

	outFile.write("};")
