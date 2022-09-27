#https://eprint.iacr.org/2015/247.pdf
from sage.combinat.gray_codes import combinations


def gen_sparse_prime(highpow, base, pad, weight):
	flag=False;
	res=[2^512,1,1];
	target=384;
	
	permut=Permutation[pad-1..1];
	
	b =[1..pad];
	for i in [0..pad-1]:b[i]=0;
	
	for i,j in combinations(pad,weight):
		
		b[i] = 0; b[j] = 1;
		
		element=0;
		for k in [1..pad]:
			element=element+b[k-1]*2^(k-1);
		p=1+(element)*(2^base)+2^(highpow);
		
		if(is_pseudoprime(p)==true):
			print( hex(p));
				
	return p;
gen_sparse_prime(255, 128, 127, 1)  ;
                                                                                                                                                               



########################################
#compute bls12 params from seed u
def bls12_params(u):
	core=((u-1)^2*(u^4-u^2+1));	
	p=(((u-1)^2)*(u^4-u^2+1))/3+u;
	q=u^4-u^2+1;
	return [p, q, core];

########################################
def bn_params(u):
	p=36*u^4+36*u^3+24*u^2+6*u+1;
	q=36*u^4+36*u^3+18*u^2+6*u+1;
	return [p,q,0];
########################################
def gen_bn_stark(base, pad, start):
	j=start;
	flag=False;
	
	
	
   						
	while(flag==False):
		
		
		
		u=((2^pad)+j)*(2^base);
		print("j=", j, "u=",u);
		[p,q,a]=bn_params(u);
		j=j+1;
		
		
		if(is_prime(p)==true): 
			flag=true;
			if(is_prime(q)==false): flag=false;
		
		
	print("size of p:", round(log(p)/log(2)));
	return [p,q,u];


########################################
def gen_bn_stark2(base, pad, weight):
	flag=False;
	res=[2^512,1,1];
	target=384;
	
	permut=Permutation[pad-1..1];
	
	b =[1..pad];
	for i in [0..pad-1]:b[i]=0;
	
	for i,j in combinations(pad,weight):
		print("res=",res);
		b[i] = 0; b[j] = 1;
		print(b);
		element=0;
		for k in [1..pad]:
			element=element+b[k-1]*2^(k-1);
		u=(element)*(2^base);
		print("k=", element, "u=",u);
		[p,q,a]=bn_params(u);
		if(is_pseudoprime(p)==true):
			flag=true;
			if(is_pseudoprime(q)==false): flag=false;
		if (flag==true):
			if(bitsize(p)<=target): 
				res=[p,q,u];
			
	return res;
		

########################################	
def gen_bls_stark2(base):

	start_j=2^3;
	j=3*start_j;
	flag=False;
	bn_p=bn_q=0;
	cpt_i=0;
	pad=18
	while(flag==False):
		cpt_i+=1;
		print("\n Iteration",cpt_i, "seed=",j);
		u=(j)*(2^(base+pad))+1;
		core=((u-1)^2*(u^4-u^2+1));

		bn_p=((u-1)^2*(u^4-u^2+1))/3+u;
		bn_pm1=((u-1)^2*(u^4-u^2+1))/3+(u-1);
		bn_q=u^4-u^2+1;
		print( "core mod 3",mod(core,3));
		if(mod(core,3)!=0): return [0,0,0];
		print( "primality bn_q:",bn_q, is_pseudoprime(bn_q));
		print( "primality bn_p:",bn_p, is_pseudoprime(bn_p));
		print(Mod(bn_p, 2^base));
	#print (factor(bn_p));	
	#print(" NTT compliance:",Mod(bn_p-1, 2^32));
		j=3*(randint(1,16384));
		#flag=true;
		print("p=", bn_p);
	#bn_p=randint(2^383,2^384);
		if(is_pseudoprime(bn_p)):flag=true;
		if(is_pseudoprime(bn_q)==false):flag=false;
	
	print("size of p:", round(log(bn_p)/log(2)));	
	return [ bn_p,bn_q,u]



########################################
def bitsize(n):
	return round(log(n)/log(2));
########################################
def gen_bls_stark(base):

	start_j=2^3;
	j=3*start_j;
	flag=False;
	bn_p=bn_q=0;
	cpt_i=0;
	pad=32;
	while(flag==False):
		cpt_i+=1;
		print("\n Iteration",cpt_i, "seed=",j);
		u=(j)*(2^(base))+1;
		core=((u-1)^2*(u^4-u^2+1));

		bn_p=((u-1)^2*(u^4-u^2+1))/3+u;
		bn_pm1=((u-1)^2*(u^4-u^2+1))/3+(u-1);
		bn_q=u^4-u^2+1;
		print( "core mod 3",mod(core,3));
		if(mod(core,3)!=0): return [0,0,0];
		print( "primality bn_q:",bn_q, is_pseudoprime(bn_q));
		print( "primality bn_p:",bn_p, is_pseudoprime(bn_p));
		print(Mod(bn_p, 2^base));
	#print (factor(bn_p));	
	#print(" NTT compliance:",Mod(bn_p-1, 2^32));
		j=3*(randint(1,2^pad));
		#flag=true;
		print("p=", bn_p);
	#bn_p=randint(2^383,2^384);
		if(is_pseudoprime(bn_p)):flag=true;
		if(is_pseudoprime(bn_q)==false):flag=false;
	
	print("size of p:", round(log(bn_p)/log(2)));	
	return [ bn_p,bn_q,u]

########################################
#gen_bls_stark(32);

#h u0 = 262+259+255+215+210−1
u0=2^62+2^59+2^55+2^15+2^10-1;
[tp, tq, tu0]=bn_params(u0);
#test that we can regenerate the well known bn254
print("test gen bn:",is_prime(tp) and is_prime(tq));
#test that we can regenerate the reference  bls12
# u0 = −2^106 − 2^92 −260 −234 + 212 −29 
u0=-2^106-2^92-2^60-2^34+2^12-2^9;
[tp, tq, tu0]=bls12_params(u0);
print("test gen bls12:",is_pseudoprime(tp) and is_pseudoprime(tq));

## generate a NTTP-F bn curve
#[p1,q1,u1]=gen_bn_stark(32, 30,0);#253 bits
[p15,q15,u15]=gen_bn_stark(32, 62,0);#61=377, 62=381, 63=385


#[p2,q2,u2]=gen_bn_stark2(32, 62,3);
#[p2,q2,u2]=gen_bn_stark2(64, 31,4);

#[p2,q2,u2]=gen_bn_stark(64, 30,0);#generate a BN curve for NTT-64, size is 381
#[p3,q3,u3]=gen_bls_stark2(32);


#generating a bls12-377 stark friendly
#[p4,q4,u4]=bls12_params(u3);
#bls12, taille 377
#305219500475757166228558390863964595760112450706650209169686672304182421640255417838826141428879168109621203173377 9429507739605422367293385796761183156533834559708520556320901042060226723841 9854220033041891329 377

#p= 489310887709731879223026037286425976099657111561645031290321808778637844817894803043937927188582344810613394374657
#489310887709731879223026037286425976099657111561645031290321808778637844817894803043937927188582344810613394374657 12916279156389480991645078327673546215494933353126673295865306472112308879361 #10660666457160941569 378

#15066331200865959937:381
#19514487258621149185:383
#21784008893348708353:384

