# Animation-Player
Animation player that takes in a .skel, .skin, and .anim file and presents a multi-jointed character in motion

The .skel file will provide information to the player about the character's joints. The .skin file will provide information to the player about the skin mesh of the character that will be wrapped around the joints. The .anim file will provide information to the player about how the character should move.

Usage: /menv file1.skel file2.skin file3.anim

Example files (wasp.skel, wasp.skin, wasp_walk.anim) are provided that give you a looped animation clip of a wasp walking.
tube.skel and tube.skin correspond to a basic character with two joints that you can use sample.anim to animate.
test.skel and dragon.skel are files that contain the joint information of two other characters but do not have any .skin or .anim files associated with them.

You must have a .skel file to be able to run it. If you only run it with a .skel file and no others, it will present you with the skeleton of the character.
You may also run it with a .skel and a .skin file only which will give you the whole character in a static pose.
In both of these cases, you can move the individual joints of the characters using your keyboard.
