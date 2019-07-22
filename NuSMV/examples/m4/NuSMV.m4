divert(-1) #do not output anything (i.e. end-of-line after definitions)

#--*****************************************************************************
#--Copyright (C) 2002-2003 Roger Villemaire <villemaire.roger@uqam.ca>.
#--All rights reserved.
#--
#--This file is free software; you can redistribute it and/or modify it
#--under the terms of the GNU Lesser General Public License as published
#--by the Free Software Foundation; either version 2.1 of the License, or
#--(at your option) any later version.
#--
#--This file is distributed in the hope that it will be useful, but
#--WITHOUT ANY WARRANTY; without even the implied warranty of
#--MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#--Lesser General Public License for more details.
#--
#--You should have received a copy of the GNU Lesser General Public
#--License along with this file in the file "COPYING.LIB"; if not, write
#--to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
#--Boston, MA 02111-1307 USA
#--*****************************************************************************

#-- This file contains some useful m4 macros for NuSMV <http://nusmv.fbk.eu>
#-- In order to use these macro inside a NuSMV file your must start it
#-- by `include(NuSMV.m4)dnl' without the quotes. Furthermore if your 
#-- NuSMV version has m4 support you must give the -m4 options to NuSMV to 
#-- preprocess the file through m4. This means that m4 must be installed on 
#-- your system. To give options to m4 you can use the NuSMV option 
#-- -m4options <M4_OPTIONS>. See the m4 info page ("info m4") for the possible m4 
#-- options.
#-- 
#-- WARNING: Make sure that this file is not included twice, since otherwise
#--          m4 will get confused.
#--*****************************************************************************


#-------------------------------------------------------------------------------
#--change the quote characters from macro.m4 of <etienne.gagnon@uqam.ca>
#-------------------------------------------------------------------------------

#--The usual m4 quote creates problems in NuSMV file. A closing quote inside
#--a NuSMV or m4 comment closes the last open quote. 

define(`left_quote', `[[:::]')
define(`right_quote', `[:::]]')
changequote(`[:', `:]')


#-------------------------------------------------------------------------------
#--Uncomment the following if you want to have m4 treat NuSMV comments as m4 comments.
#--This means that in that case there will be no macro expansions in NuSMV comments.
#--Personnaly I find it useful to leave m4 expansion inside NuSMV comments in order
#--to print context information usuful for debugging.
#--changecom([:--:])  #take the same comment delimiters as NuSMV


#-------------------------------------------------------------------------------
#-- This macro expands to a message warning that the file
#-- was generated by m4
#-- Put this at the beginning of a file using m4 macros if you use
#-- m4 directly and want this message in the generated file.
define([:nusmv_m4_auto_gen:],
	[:-----------------------------------------------------------------------------
-- WARNING-WARNING-WARNING-WARNING-WARNING-WARNING-WARNING-WARNING WARNING --
-- THIS FILE WAS AUTOMATICALLY GENERATED BY m4 FROM FILE __file__
-- YOU SHOULD MODIFY THE ORIGINAL FILE.                                    --
-----------------------------------------------------------------------------:])


#-------------------------------------------------------------------------------
#-- This macro return the first index of a range.
#-- So first(A..B) expands to A.
define([:first:],[:substr($1,0,index($1,[:.:])):])


#-------------------------------------------------------------------------------
#-- This macro return the last index of a range.
#-- So last(A..B) expands to B.
define([:last:],[:substr($1,eval(index($1,[:.:])+2)):])


#-------------------------------------------------------------------------------
#-- This forloop is taken from the info page with some
#-- added validations.
#-- forloop(I,FIRST,LAST,EXPRESSION(I))
#-- expands to EXPRESSION(FIRST)EXPRESSION(FIRST+1)...EXPRESSION(LAST)
#-- First we test that I is a name.
define([:forloop:],
	[:ifelse(isname($1),[:1:],[:_forloop1($@):],
	[:errprint([:m4::]__file__:__line__ [::Argument 1 of forloop must be a name
:])m4exit(1):]):])
#-- Second we test that FIRST and LAST are numbers
define([:_forloop1:],
	[:ifelse(eval(isnumber($2) && isnumber($3)),[:1:],[:_forloop2($@):],
	[:errprint([:m4::]__file__:__line__[::Argument 2 and 3 of forloop must be numbers
:])m4exit(1):]):])
#-- Third if FIRST > LAST there is nothing to produce
define([:_forloop2:],
	[:ifelse(eval($2 > $3),1,[::],[:_forloop3($@):]):])
define([:_forloop3:],
	[:pushdef([:$1:], [:$2:])_forloop([:$1:], [:$2:], [:$3:], [:$4:])popdef([:$1:]):])
define([:_forloop:],
        [:$4[::]ifelse($1, [:$3:], ,
                [:define([:$1:], incr($1))_forloop([:$1:], [:$2:], [:$3:], [:$4:]):]):])


#-------------------------------------------------------------------------------
#-- A name is made of letters, digits and _ not starting by a digit.
define([:isname:],[:eval(regexp($1,[:\`[a-zA-Z][a-zA-Z0-9_]*\':]) = 0):])
define([:isnumber:],[:eval(regexp($1,[:\`[0-9]*\':]) = 0):])


#-------------------------------------------------------------------------------
#-- forlooprange(I,A..B,EXPRESSION(I)) expands to
#-- EXPRESSION(A)EXPRESSION(A+1)..EXPRESSION(B)
#-- I must be a name
define([:forlooprange:],[:forloop($1,first($2),last($2),$3):])



#-------------------------------------------------------------------------------
#-- select(C,I,A..B,EXPRESSION(I)) expands to
#-- case
#--   C = A   : EXPRESSION(A);
#--   C = A+1 : EXPRESSION(A+1);
#--   ...
#--   C = B   : EXPRESSION(B);
#-- esac

define([:select:],[:
    case
forlooprange($2,$3,[:[:     :]:]$1 = $2 : $4;
)    esac:])

divert[::]dnl
