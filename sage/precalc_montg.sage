#/**********************************************************************************/
#/* Copyright (C) 2022 - This file is part of cy_lib project				 	   */
#/* License: This software is licensed under a dual BSD and GPL v2 license. 	   */
#/* See LICENSE file at the root folder of the project.							   */
#/* FILE: precalc_montg.sage										   		       */
#/* 																			   */
#/* 																			   */
#/* DESCRIPTION: computation of Montgomery constants							   */
#/**********************************************************************************/
####################################		
def precompute_montgomery_constants(n, size):
	Fp=FiniteField(n)
	tn=ceil(ceil(log(n)/log(2))/size)
	print("tn=",tn)
	R=(2^size)^tn;
	n0=mod(-n, 2^size)^-1
	Rm1 = Fp(2^size)^-1
	
	print("n0=", n0, hex(n0))
	print("R^-1 =", Rm1, hex(Rm1))
	
	return [lift(n0), lift(Rm1)];		
	

#/*********** Write as machine words ************/ 
def Conv_word(A, size_word):
	sizeA=ceil(log(A)/log(2))  
	sizeM=ceil(sizeA/size_word)
	M=copy(matrix(1, sizeM)[0])
	
	mask = 2^size_word-1
	print("{");
	for i in [0..sizeM-1]:
		M[i]= A 	& mask;
		print(" ",hex(M[i]),",");
		A = A >> size_word
	print("};");
	return M;	
	
#sec256k1 modulus
n=0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F
name="p_256k1"
#precompute for a 128 bits emulation by 64 bit words
res=precompute_montgomery_constants(n, 128);
Conv_word(res[0], 64);
Conv_word(res[1], 64);


