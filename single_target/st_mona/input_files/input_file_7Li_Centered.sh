#But if include a delimiter and try to fool it what will happen = ?
#How about a delimiter = and a newline inidcator?;
#Ok so what happens is it accepts it, and may even convert it to double 
#but in the end there is no variable to assign it to so it does affect anything.
#UNLESS THE SENTENCE IS TOO LONG! (1 sentence = everything up to a ;
#a blank line is bad
#INFO("Using default values for experiment e12004, d,p reaction");
	reacType	=	"glaub";        #"no reaction, d,p"; 
	eBeam		=	79.85; # do poundsigns afterwards affect anything?
	beamA		=	7; #lets really test this
	bogus 		= 	1337;
	beamZ		=	3; # comment all the lines
	dTarget 	=	376; #they do you should be really careful!
	dEbeam		=	0.006; # did i change this? 0.0215 0.005215
	resTime		=	0.3; # i dont remember
	nNeutr		=	0; # why zero?
	nProt		=	0; # why not zero?
	bSpotCx		=	0.004; #what is zero anyways?
	bSpotCtx	=	0.001; #so far my code dosen't seem to care
	bSpotCy		=	-0.008; #i bet if i make a really long comment it will explode though
	bSpotCty	=	-0.002; # for real
	bSpotDx		=	0.009; #for real real  0.003
	bSpotDy		=	0.0034; #actually they dont, i think you're ok,0.0055 0.0045
	bSpotDtx	=	0.007; #antoher test of comments 0.006
	bSpotDty	=	0.004; 
	crdc1MaskLeft	=	0.15;
	crdc1MaskRight	=	-0.15; # i am fairly certain i can put anything in a coment here...
	crdc2MaskLeft	=	0.15; 
	crdc2MaskRight	=	-0.15;
	crdc2MaskTop	=	0.15; 
	crdc2MaskBot	=	-0.15;
	crdc2dist	=	1.55;
	cosymap		= 	"m7Li_e11028"; #"m24O"
	fragA		=	7; #24
	fragZ		=	3; #8
	targA		=	9; #2
	targZ		=	4; #1
	maxbulge 	= 	0.0000000000000001; #[m]
	tflatfoil	=	0.0000000000000001; #0
