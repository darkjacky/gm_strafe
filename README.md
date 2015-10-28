# gm_strafe
Garrys mod module for gamemodes like bunny hop or deathrun. This module should replace the Server side of GM:Move that is allows strafing.


Compile it for yourself since you need to adit some values. The current add speed = 35. That is an extreme amount.

This module should replace the GM:Move on the server. So clients still need to lua variant.

Benchmark test:
Lua Script ~ 0.00001182788 per call
c++ module ~ 0.00000693849 per call

If you find anything that can be improved please tell me.
