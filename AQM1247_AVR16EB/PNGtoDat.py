from PIL import Image
start = 394

def png_to_hex(filename):
    img = Image.open(filename).convert("RGBA")

    width, height = img.size
    result = []

    for atai in range(4):
        for x in range(start, start+20):
            value = 0
            for y in range(atai*8, atai*8+8):
                if y < height:
                    r, g, b, a = img.getpixel((x, y))
                    if (r, g, b) == (0, 0, 0):
                        value |= 1 << (y%8)
            result.append(value)
    return result

data = png_to_hex("font_width.png")
print(", ".join(str(v) for v in data))
