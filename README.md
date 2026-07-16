# RotaryCalbasic
The calculator project instead of numpad i just use rotary encoder :3

I always wanted to try the knob (rotary encoder) and I think its now the chance to try it!!!

![Rotary Encoder](Media/photo_6253371047977619114_y.jpg)

First I need to understand the concept of the rotary encoder first. (Well because this is the forst time I ever use this)

When the rotary encoder get rotate, both metal contact under the disk will be touched (IDK what to say) slightly off. When it rotate clockwise the metal A will contact before metal B and t goes in reverse for counter clockwise. So, due to that I wrote that 
```ino
if (knobmov) { // Check falling on pin A
    if (!knobvalueb) // Check status pin B
        CW // Contact A when B is not contacted
    else
        CCW // Contact A when B is already contacted
}
```

The display system use ["u8g2"](https://github.com/olikraus/u8g2) libary and i use [lopaka](https://lopaka.app)

This is the short version? ![idk](Media/RCB1.png)

## Devlog
**14|07|2026** :: Made the selection working :3 next is the calculation

Well i did it (version 1.0) it have some basic calculation (+. -, *, /, ^, (, ))

rn it use `=` for result and `#` for delete the recent character

the math libary it use is `tinyexpr.h` it is very handy for some thing that require this less memory and storage ( By now 88% storage and 66% memory ) 

**16|07|2026** I upgraded the function in to version 1.2
