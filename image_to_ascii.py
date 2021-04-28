import os
import ascii_magic


pwd = os.path.abspath(os.getcwd())
path = os.path.join(pwd, 'frames', 'images')
save_path = os.path.join(pwd, 'frames', 'ascii_text')

frames = sorted([os.path.join(path, f) for f in os.listdir(path) if os.path.isfile(os.path.join(path, f))])

for i in range(len(frames)):
    name = f'frame_0{i}.txt' if i < 10 else f'frame_{i}.txt'
    curr_path = os.path.join(save_path, name)
    output = ascii_magic.from_image_file(frames[i])
    with open(curr_path, 'w') as f:
        f.write(output)
