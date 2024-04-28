
# This is a demo for forward (based on Gaussian beam method)
# ChangZhimiao, 2023/11/13
# changzm23@mails.jlu.edu.cn

# Before you use forward program, you should define the velocity file and head file
# This demo gives a program to generate the head file (program: makehead)
# Or you can use the data in data folder

# head file parameters
ntr=737
dtr=12.5
ftr=0
dftr=0

nshot=3
dshot=3000
fshot=0

# forward parameters
naper=150
nt=750
nx=737
nz=750
dt=0.004
dx=12.5
dz=4
amax=50
vfile=../../data/vel_marmousi # input
reftfile=reflectivity.dat # output
orgfile=shot.dat # output
headInfo=headInfo # input

shotfilesu=shot.su

# generate the head file
../../bin/makehead ntr=$ntr dtr=$dtr ftr=$ftr dftr=$dftr \
    nshot=$nshot dshot=$dshot fshot=$fshot 

# generate the seismic file
../../bin/forwardGBM naper=$naper nt=$nt nx=$nx nz=$nz dt=$dt dx=$dx dz=$dz amax=$amax nshot=$nshot\
    reftfile=$reftfile orgfile=$orgfile vfile=$vfile headInfo=$headInfo \
    ntr=$ntr dtr=$dtr

../../bin/data2su nt=$nt nshot=$nshot dtr=$dtr dt=$dt headInfo=$headInfo shotfile=$orgfile shotfilesu=$shotfilesu

# SU ximage (be sure you have SU installed)
ximage < $orgfile n1=$nt 
