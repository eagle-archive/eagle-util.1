
// Read from (x,y,z) points from CSV
[x,y,z]=textread('Z:\Host_C\VmTemp\grid.csv','%f,%f,%f');
foo = fit([x,y], z, 'cubicinterp');
plot(foo);

[x,y,z]=textread('Z:\Host_C\VmTemp\grid-tps.csv','%f,%f,%f');
foo2 = fit([x,y], z, 'cubicinterp');
plot(foo2);
