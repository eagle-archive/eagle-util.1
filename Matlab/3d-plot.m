
// Read from (x,y,z) points from CSV
// 13295,27187,0
[x,y,z]=textread('Z:\Host_C\VmTemp\grid.csv','%d,%d,%d');

foo = fit([x,y], z, 'cubicinterp');
plot(foo);
