/**********************************************************************************/
/* Copyright (C) 2022 - Renaud Dubois											  */
/* This file is part of cy_lib project						 					  */
/* License: This software is licensed under a dual BSD and GPL v2 license. 	      */
/* See LICENSE file at the root folder of the project.				 			  */
/* FILE: cy_arithmetic_component.h								                  */
/* 																			      */
/* 																			      */
/* DESCRIPTION: defining arithmetic component in an object-C way                  */
/* this structure can be used for rust bindings									  */
/**********************************************************************************/

#ifndef CY_ARITHMETIC_COMPONENT_H_
#define CY_ARITHMETIC_COMPONENT_H_

 #include "cy_errors.h"

typedef cy_error_t (*Fct_Arithm_Function_t) (void *i_ps_Arg1, void *i_ps_Arg2,  void *o_ps_Res);

typedef cy_error_t (*Fct_Arithm_Convert_t) ( const uint8_t *i_initializer, size_t i_initializer_t8,  void *o_ps_Res);
typedef cy_error_t (*Fct_Arithm_UnConvert_t) ( const void *o_ps_Res, const uint8_t *o_out, size_t *o_out_t8);

typedef cy_error_t (*Fct_Arithm_Init_t) (void *i_p_ctx, uint8_t *initializer, size_t initializer_t8);
typedef cy_error_t (*Fct_Arithm_UnInit_t) (void *i_p_ctx);


uint8_t *Sec_RAMp;


struct cy_ari_generic_field_s{
  uint8_t *Sec_RAMp; /* pointer between the Shared RAM and the component*/

  uint64_t                         id;                   		/**< Component identifier                                   */
  unsigned char					   Component_name[MAX_SIZENAME];/*

  /*Arithmetic functions*/
  Fct_Arithm_Init_t					*Arithm_Init;
  Fct_Arithm_Function_t             *Arithm_Add;               	/**< pointer to add function                   */
  Fct_Arithm_Function_t             *Arithm_Sub;               	/**< pointer to sub function                   */
  Fct_Arithm_Function_t             *Arithm_Double;             /**< pointer to the double function                   */
  Fct_Arithm_Function_t             *Arithm_Mul;               	/**< pointer to the mul function                   */
  Fct_Arithm_Function_t             *Arithm_Expo;               /**< pointer to the expo function                   */
  Fct_Arithm_Function_t             *Arithm_Inv;               	/**< pointer to the inv function                   */
  Fct_Arithm_Function_t             *Arithm_Red;               	/**< pointer to the reduction function                   */
  Fct_Arithm_Function_t             *Arithm_Sqrt;               /**< pointer to the sqrt function                   */
  Fct_Arithm_Function_t             *Arithm_Div;               	/**< pointer to the division function                   */
  Fct_Arithm_UnInit_t				*Arithm_UnInit;
  /*IO functions*/
  Fct_Arithm_Convert_t				*Convert_u8toObj;
  Fct_Arithm_UnConvert_t			*Convert_Objtou8;

  void *ctx;
  void *opt;
};

typedef struct cy_ari_generic_field_s cy_arithm_Field_Component_t;




#endif /* SRC_OBJECTC_CY_ARITHMETIC_COMPONENT_H_ */
