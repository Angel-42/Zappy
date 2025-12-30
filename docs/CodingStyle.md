# C CODING STYLE

## KEEP YOUR CODE NICE AND CLEAN


## C CODING STYLE

The _Epitech C Coding Style_ is a set of rules that have been created within the school, and that
you have to follow.

It covers various aspects of programming in C, from the overall organization of the repository to
the individual lines of code.

It is compulsory on all programs written in C as part of Epitech’s projects, **regardless** of the year
or unit, as long as a language to program in is imposed.

It applies to **all source** (.c) and **header files** (.h) present in the repository, as well as **Makefiles**.

Adopting a coding style makes reading code written by others easier. As such, it facilitates group
work, as well as help given to you by the educational team and the assistants.

It is also an excellent way to encourage structuring the code and making it clearer, and thus
facilitates:

```
✓ its reading;
✓ its debugging;
✓ its maintenance;
✓ its internal logic definition;
✓ its reusability;
✓ writing tests;
✓ adding new features;
✓ and even more...
```
```
A clean and structured code always feels nice to look at, so give yourself this treat. ;)
```
```
When you are facing a choice and you do not know what decision to make, always ask
yourself which one helps you make your code clearer, ergonomic and flexible.
```
In case of uncertainty or ambiguity regarding the principles and rules specified in this document,
please refer to your local educational manager.

```
Any attempt to bypass the coding style rules will result in an entire project invalida-
tion.
```

Rules are categorized into 4 severity levels: **fatal** , **major** , **minor** and **info**.

**Fatal rules** are related to the **objective itself of programming in C**. Violating **even once** a
fatal rule will make your project rejected and **not evaluated at all**.

**Major rules** are related to the **structure of the code** and to practices that are detrimental to
the production of a code of good quality. Violating **any** of the major rules (even once) is a **major
problem** and must be corrected as a priority concern.

**Minor rules** are generally related to the **visual presentation of the code** , which can make
the code difficult to read if not followed consistently. Repeatedly violating minor rules must be
avoided, as it creates inconsistently formatted code, which in turn makes it harder to read.

**Info rules** are related to **specific trivial points** that are not as important as other rules. Each of
these rules are however anchored in good practices, and should as such be followed to ensure a
code of the best quality possible.

There are many and many ways to produce unclean code, and as such many rules to follow in
order to avoid them.
Even though one cannot mention all of them in this document, they still have to be respected.
We call them _implicit rules_ when not explicitly defined in this document.

```
Implicit rules are considered as info.
```
```
The Coding Style is a purely syntactic convention, so it can not be used as an excuse if your
program does not work. ;)
```
```
Although following the coding style is not required in all projects, this is not a reason for
not always sequencing and structuring your code.
Most of the rules in this coding style can be applied to all languages, so they can be useful
when you are doing projects in different languages.
```
```
It is easier and quicker to follow the coding style from the beginning of a project rather
than to adapt existing code at the end.
```
```
This document is inspired by the Linux Kernel Coding Style, and is freely adapted from
Robert C. Martin’s excellent book Clean Code.
```

## Banana

The adherence to the coding style is **partially** checked during evaluations by a tool called the
_Bot Analyzing Nomenclature And Nonsensical Arrangements_ , better known as _Banana_.
You can (and should) also use this tool to check that your code follows a good portion of the rules.
Other rules are checked manually, with the great tool that are your eyes.

The rules are tagged with three possible levels of support by Banana:

```
✓ : the rule is completely checked by Banana;
✓ : the rule has to be checked manually (Banana does not support it);
✓ : some parts of the rule are checked by Banana, other parts have to be checked manually.
```
### Using Banana

Using Banana is very simple:

1. Ensure that Docker is installed on your machine.
2. Clone the Epitech coding style checker scripts repository.
3. Launch the appropriate script depending on your operating system (coding-style.shfor Unix-
based OS,coding-style.ps1for Windows).
4. You have your results!

```
The script will make sure that you always have the latest version of Banana.
```
### Banana source repository

The Banana source repository is publicly accessible.

If you find any problem or have any question regarding Banana, you can open an issue there,
and a Banana developer will happily answer you.

You can even contribute to Banana yourself if you want! ;)


## C-O - Files organization

### C-O1 - Contents of the repository

The repository **must not** contain **compiled** (.o,.a,.so,... ), **temporary** or **unnecessary** files (*~,#*#,
etc.).

```
Git has a wonderful way to help you keep your repository clean. ;)
```
### C-O2 - File extension

Sources in a C program must **only** have.c **or** .hextensions.

### C-O3 - File coherence

A source file must match a **logical entity** , and group all the functions associated with that entity.

Grouping functions that are **not related** to each other in the same file has to be **avoided**.

You are allowed to have **10 functions** (including at most **5 non-static functions** ) in total per file.

```
Beyond these amounts, you must subdivide your logical entity into several sub-entities.
```

### C-O4 - Naming files and folders

The name of the file must define the logical entity it represents, and thus be **clear, precise,
explicit and unambiguous**.

```
For example, files likestring.coralgo.care probably incorrectly named.
Names likestring_toolbox.corpathfinding.cwould be more appropriate.
```
All file names and folders must **be in English, according to the** snake_case **convention** (that is,
only composed of lowercase, numbers, and underscores).

```
Abbreviations are tolerated as a way to significantly reduce the size of a name only if it
does not lose its meaning.
```

## C-G - Global scope

### Multiline statements

Multiline statements are allowed.

Here are examples of properly segmented multiline statements:

b o o l i s _ b e t w e e n ( u n s i g n e d i n t n , u n s i g n e d i n t l o w _ b o u n d ,
u n s i g n e d i n t h i g h _ b o u n d , c o n s t c h a r * f a i l _ m e s s a g e ) ;

i n t m a i n ( v o i d )
{
p r i n t f ( " [ % s ] % s : % d \ n " , g e t _ e l e m e n t _ t y p e ( ) , g e t _ e l e m e n t _ n a m e ( ) ,
g e t _ e l e m e n t _ v a l u e ( ) ) ;
m y _ p u t s t r ( " W r i t i n g m u l t i l i n e s t a t e m e n t s i n C i s e a s y , "
" y o u j u s t n e e d t o b r e a k t h e l i n e , a n d y o u a r e d o n e! " ) ;
i f ( c a l l _ t o _ a _ f u n c t i o n _ w i t h _ a _ l o n g _ b u t _ d e s c r i p t i v e _ n a m e ( )
& & ( c a l l _ t o _ a n o t h e r _ f u n c t i o n _ i n s i d e _ p a r e n t h e s e s ( )
| | c a l l _ t o _ f u n c t i o n _ i f _ t h e _ f i r s t _ o n e _ d i d _ n o t _ s u c c e e d ( ) )
& & f i n a l _ c a l l _ t o _ f u n c t i o n _ t o _ d e m o n s t r a t e _ m u l t i l i n e ( ) ) {
i _ a m _ a _ t e a p o t ( 4 1 8 ) ;
}
}

```
Do not use the backslash character ( \ ) to break lines in C files, because it will only visually
break the line.
As such, you will get into trouble regarding the coding style’s C-G6 rule!
```
### Language extensions

Language extensions are **not supported**.

Using them might lead to an undefined behaviour by Banana and, as such, to problems in your
evaluation.

Some examples are:

```
OK Not supported
[[noreturn]] __attribute__((noreturn))
#ifndef/#define/#endif #pragma once
\x1B \e
```

### C-G1 - File header

C files (.c,.h,... ) and every Makefiles must always start with the **standard header** of the school.
This header is created in Emacs using theCtrl + candCtrl + hcommand.

For C files:

/ *
* * E P I T E C H P R O J E C T , [ Y E A R ]
* * [ N A M E _ O F _ T H E _ P R O J E C T ]
* * F i l e d e s c r i p t i o n :
* * N o f i l e t h e r e , j u s t a n e p i t e c h h e a d e r e x a m p l e.
* * Y o u c a n e v e n h a v e m u l t i p l e l i n e s i f y o u w a n t!
* /

For Makefiles:

# #
# # E P I T E C H P R O J E C T , [ Y E A R ]
# # [ N A M E _ O F _ T H E _ P R O J E C T ]
# # F i l e d e s c r i p t i o n :
# # N o f i l e t h e r e , j u s t a n e p i t e c h h e a d e r e x a m p l e.
# # Y o u c a n e v e n h a v e m u l t i p l e l i n e s i f y o u w a n t!
# #

```
Always add a meaningful description of the file, you have a unlimited amount of line to
do so.
```
### C-G2 - Separation of functions

Inside a source file, implementations of functions must be separated by **one and only one
empty line**.


### C-G3 - Indentation of preprocessor directives

The preprocessor directives must be **indented according to the level of indirection**.

```
Indentation must be done in the same way as in the C-L2 rule (groups of 4 spaces, no tab-
ulations).
However , preprocessor directives must be indented independently of all the other
code.
```
# i f n d e f W I N 3 2
# i n c l u d e < s t d b o o l. h >
# i f d e f i n e d ( _ _ i 3 8 6 _ _ ) | | d e f i n e d ( _ _ x 8 6 _ 6 4 _ _ )
c o n s t s i z e _ t P A G E _ S I Z E = 4 0 9 6 ;
# e l s e
# e r r o r " U n k n o w n a r c h i t e c t u r e "
# e n d i f

s t r u c t c o o r d s {
i n t x ;
i n t y ;
} ;
# e n d i f

### C-G4 - Global variables

Global variables must be **avoided** as much as possible.

Only global **constants** should be used.

```
A constant is considered as such if and only if it is correctly marked with theconstkeyword.
Watch out, this keyword follows some particular and sometimes surprising rules!
```
c o n s t f l o a t G O L D E N _ R A T I O = 1. 6 1 8 0 3 3 9 8 8 7 5 ; / * O K * /
i n t u p t i m e = 0 ; / * C - G 4 v i o l a t i o n * /

### C-G5 - include

includedirectives must **only** include C header (.h) files.


### C-G6 - Line endings

Line endings must be **done in UNIX style** (with\n), and must **never end with a backslash (** \ **)**.

```
\rmust not be used at all, anywhere in the files.
```
```
git configcan help you keep your lines correctly ended.
```
```
Have a look at the Multiline statements section up above to see how to properly break
lines.
```
### C-G7 - Trailing spaces

**No trailing spaces** must be present at the end of a line.

### C-G8 - Leading/trailing lines

**No leading empty lines** must be present.

**No more than 1 trailing empty line** must be present.

```
Make sure you also follow the C-A3 rule.
```
### C-G9 - Constant values

**Non-trivial constant values** should be defined either as a global constant or as a macro.

This greatly helps you when you want to modify an important value in your program, because
you do not need to find all occurences of this value scattered throughout your code, and only
need to change it in one place.


### C-G10 - Inline assembly

**Inline assembly** must **never** be used.

Programming in C must be done... in C.


## C-F - Functions

### C-F1 - Coherence of functions

A function should only do **one thing** , not mix different levels of abstraction, and respect the
single-responsibility principle (a function should be changed only for one reason).

```
For example, a call tomalloc(), a call toallocate_user(), and a call tocreate_user()all have 3
different levels of abstraction.
```
### C-F2 - Naming functions

The name of a function must **define the task it executes** and must **contain a verb**.

```
For example, thevowels_nb()anddijkstra()functions are incorrectly named.
get_vowels_number()andsearch_shortest_path()are more meaningful and precise.
```
All function names must **be in English, according to the** snake_case **convention** (meaning that
it is composed only of lowercase, numbers, and underscores).

```
Abbreviations are tolerated if they significantly reduce the name without losing meaning.
```

### C-F3 - Number of columns

The length of a line must not exceed **80 columns** (not to be confused with 80 _characters_ ).

```
A tab represents 1 character, but several columns.
The line break character (\n) is part of the line, and thus counts in its length.
```
```
Even though this rule especially applies to functions, it applies to all C files, as well as
Makefiles.
```
### C-F4 - Number of lines

The body of a function should be as **short as possible** , and must not exceed **20 lines**.

i n t m a i n ( v o i d ) / * t h i s f u n c t i o n i s 2 - l i n e - l o n g * /
{
p r i n t f ( " h e l l o , w o r l d \ n " ) ;
r e t u r n 0 ;
}

```
The maximum length of a function is inversely proportional to the complexity and in-
dentation level of that function. So, if you have a conceptually simple function that is
just one long (but simple) case-statement, where you have to do lots of small things
for a lot of different cases, it’s OK to have a longer function.
— Linus Torvalds, Linux Kernel Coding Style —
```
### C-F5 - Number of parameters

A function must not have more than **4 parameters**.
Writing variadic functions is allowed, but they must not be used to circumvent the limit of 4
parameters.


### C-F6 - Functions without parameters

A function taking no parameters **must take** void **as a parameter** in the function declaration.

p h y s _ a d d r _ t a l l o c _ f r a m e ( ) ; / * C - F 6 v i o l a t i o n * /
p h y s _ a d d r _ t a l l o c _ f r a m e ( v o i d ) ; / * O K * /

```
The two syntaxes above have different meanings, and have different interesting beha-
viours.
```
### C-F7 - Structures as parameters

Structures must be passed to functions using a **pointer, not by copy**.

v o i d m a k e _ s o m e _ c o f f e e ( s t r u c t m y _ s t r u c t * b o a r d , i n t i ) ; / * O K * /

v o i d m a k e _ s o m e _ p o i s o n ( s t r u c t m y _ s t r u c t b o a r d , i n t i ) ; / * C - F 7 v i o l a t i o n * /

### C-F8 - Comments inside a function

There **must be no comment** within a function.
The function should be readable and self-explanatory, without further need for explanations in-
side it.

```
The length of a function is inversely proportional to its complexity, so a complicated func-
tion should be short.
In that case, a header comment (just above the prototype) should be enough to explain
it.
```
### C-F9 - Nested functions

Nested functions are **not allowed** , because they are an extension of the GNU C standard, and
because they greatly increase complexity.


## C-L - Layout inside a function scope

### C-L1 - Code line content

A line must correspond to **only one statement**.

```
We roughly define a statement as a being a short piece of code that either:
✓ creates/sets a variable/constant;
✓ calls a function without using its return value;
✓ checks a condition as part of anif,else if,switch,while,do while, orfor;
✓ makes the functionreturn.
```
Typical situations to avoid include:

```
✓ several assignments on the same line;
✓ several semicolons on the same line used to separate several statements;
✓ an assignment in a control structure expression;
✓ a condition and a statement on the same line.
```
The only exception to this rule is theforloop control structure, for which **one** statement is allowed
in each of the three parts (initialization, loop condition, and post-iteration operation).

```
OK C-L1 violation
a = 0; a = b = c = 0;
a++; a++; b++;
if (ptr != NULL) if ((ptr = malloc(42))!= NULL)
if (cond)return (ptr);
for (int i = 0; i < 42; i++) for (int i = j = 0; i < 42; i++)
for (int i = 0; i < 42; i++, j--)
str[i] ='A'; str[i++] ='A';
return my_strlen(name);
```
```
Ways to circumvent this rule are not allowed:
( ( a! = c ) | | ( b! = c ) ) & & p r i n t f ( " H e l l o , w o r l d! \ n " ) ;
is forbidden as it just a way to sneakily blend two statements into one:
i f ( ( a! = c ) | | ( b! = c ) ) {
p r i n t f ( " H e l l o , w o r l d! \ n " ) ;
}
```

### C-L2 - Indentation

Each indentation level must be done by using **4 spaces**.
**No tabulations** may be used for indentation.

When entering a new scope (e.g.: control structure), the indentation level must be incremented.

/ / O K
i n t m a i n ( v o i d )
{
c h a r l e t t e r = 'H';
i n t n u m b e r = 1 4 ;

i f ( l e t t e r = = 'H') {
m y _ p u t c h a r ('U') ;
} e l s e i f ( l e t t e r = = 'G') {
i f ( n u m b e r! = 1 0 )
m y _ p u t c h a r ('O') ;
e l s e {
m y _ p u t n b r ( 9 7 ) ;
}
}
}

/ / I n c o r r e c t
i n t m a i n ( v o i d )
{
i n t i ;
}

/ / I n c o r r e c t
i n t m a i n ( v o i d )
{
i f ( t r u e ) {
r e t u r n ( 0 ) ;
}
}

/ / I n c o r r e c t a n d u g l y
i n t m a i n ( v o i d )
{
c h a r l e t t e r = 'H';
i n t n u m b e r = 1 4 ;

i f ( l e t t e r = = 'H') {
m y _ p u t c h a r ('U') ;
} e l s e i f ( l e t t e r = = 'G') {
i f ( n u m b e r! = 1 0 ) {
m y _ p u t c h a r ('O') ;
} e l s e {
m y _ p u t n b r ( 9 7 ) ;
}
}
}


Block comments’ bodies can be freely indented (with spaces):
/ / T h e b l o c k c o m m e n t b e l o w i s v a l i d , e v e n t h o u g h i t i s i n d e n t e d w i t h o n l y o n e s p a c e
/ * *
* @ b r i e f S o m e t h i n g
*
* @ p a r a m p a t h
* @ r e t u r n v o i d *
* /
v o i d * s o m e t h i n g ( c o n s t c h a r * p a t h ) ;

### C-L3 - Spaces

When using a space as a separator, **one and only one space** character must be used.

```
Tabulations cannot be used as a separator.
```
Always place a **space after a comma or a keyword** ( **if it has arguments** ).

However, there must be **no spaces** between the name of a function and the opening paren-
thesis, after a unary operator, before a semicolon, or before a comma.

In the precise case of aforcontrol structure, if a semicolon inside the parentheses is not **imme-
diately** followed by another semicolon, it **must** be followed by a space.

**All binary and ternary operators** must be separated from their arguments by a **space on both
sides**.

```
returnis a keyword , butsizeofis an unary operator.
```
```
OK C-L3 violation
return 1;as well asreturn (1) return(1);
break; break ;
add_numbers(1, 2); add_numbers(1 , 2);
sum = term1 + 2 * term2; sum = term1+2*term2;
s = sizeof(struct file); s = sizeof (struct file);
for (size_t i; str[i] !='\0'; i++) for (size_t i;str[i] !='\0'; i++)
```
```
It is not possible to list all possible cases, but keep in mind that your code should be well
spaced out, though without unnecessary spaces.
```

### C-L4 - Curly brackets

**Opening** curly brackets must be **at the end of the line** , after the content it precedes, except for
functions definitions where they must be placed alone on their line.
**Closing** curly brackets must be **alone on their line** , except in the case ofelse/else if/do whilecon-
trol structures,enumdeclarations, or structure declarations (with or without an associatedtypedef).

```
In the case of a single-line scope, omitting curly brackets is tolerated, but you should think
about all the modifications you will have to make if you want to add a new statement to
the block. This can also introduce some nasty bugs!
```
i f ( c o n d ) { r e t u r n p t r ; } / * C - L 1 & C - L 4 v i o l a t i o n s * /
w h i l e ( c o n d ) { / * O K * /
d o _ s o m e t h i n g ( ) ;
}
i f ( c o n d )
{ / * C - L 4 v i o l a t i o n * /

...
} e l s e { / * O K * /
...
}
i f ( c o n d ) { / * O K * /
...
} / * C - L 4 v i o l a t i o n * /
e l s e {
...
}
i f ( c o n d ) / * T o l e r a t e d * /
r e t u r n p t r ;
i n t p r i n t _ e n v ( v o i d ) / * O K * /
{
...
}
i n t p r i n t _ e n v ( v o i d ) { / * C - L 4 v i o l a t i o n * /
...
}
s t r u c t c o o r d s { / * O K * /
i n t x ;
i n t y ;
} ;
s t r u c t c o o r d s
{ / * C - L 4 v i o l a t i o n * /
i n t x ;
i n t y ;
} ;

```
Even though this primarily applies to the contents of functions, this rule also applies to
code outside functions , including header files’.
```

### C-L5 - Variable declarations

Variables must be declared **at the beginning of the function**.
**Only one variable** must be declared per statement.

Theforcontrol structures may also optionally declare a variable in their initialization part.

```
Nothing prevents you from declaring and assigning a variable on the same line.
```
l o n g c a l c u l a t e _ g c d ( l o n g a , l o n g b )
{
l o n g b i g g e s t , s m a l l e s t ; / * C - L 5 v i o l a t i o n * /

b i g g e s t = M A X ( a , b ) ;
s m a l l e s t = M I N ( a , b ) ;
l o n g r e s t ; / * C - L 5 v i o l a t i o n * /
w h i l e ( s m a l l e s t > 0 ) {
r e s t = b i g g e s t % s m a l l e s t ;
b i g g e s t = s m a l l e s t ;
s m a l l e s t = r e s t ;
}
r e t u r n a ;
}

i n t m a i n ( v o i d )
{
i n t f o r t y _ t w o = 4 2 ; / * O K * /
i n t m a x = 1 2 ; / * O K * /

f o r ( u n s i g n e d i n t i = 0 ; i < m a x ; i + + ) { / * O K * /
c a l c u l a t e _ g c d ( f o r t y _ t w o , m a x ) ;
}
r e t u r n 0 ;
}


### C-L6 - Blank lines

A blank line must **separate the variable declarations from the remainder** of the function.
No other blank lines must be present in the function.

i n t s y s _ o p e n ( c h a r c o n s t * p a t h )
{
i n t f d = t h r e a d _ r e s e r v e _ f d ( ) ;
s t r u c t f i l e h a n d l e r * f h a n d l e r = N U L L ;
/ * O K * /
i f ( f d < 0 ) {
r e t u r n - 1 ;
}
i f ( f s _ o p e n ( p a t h , & f h a n d l e r ) ) {
t h r e a d _ f r e e _ f d ( f d ) ;
r e t u r n - 1 ;
}
/ * C - L 6 v i o l a t i o n * /
t h r e a d _ s e t _ f d _ h a n d l e r ( f d , f h a n d l e r ) ;
r e t u r n f d ;
}

```
No blank line is necessary if there are no variable declarations in the function.
```

## C-V - Variables and types

### C-V1 - Naming identifiers

All identifier names must **be in English, according to the** snake_case **convention** (meaning it is
composed exclusively of lowercase, numbers, and underscores).

The type names defined withtypedefmust **end with** _t.
The names of **macros and global constants** and the content of **enums** must be written in
UPPER_SNAKE_CASE.

# d e f i n e I S _ P A G E _ A L I G N E D ( x ) (! ( ( x ) & ( P A G E _ S I Z E - 1 ) ) ) / * O K * /
e n u m a r c h { / * O K * /
I 3 8 6 = 0 ,
X 8 6 _ 6 4 ,
A R M ,
A R M 6 4 ,
S P A R C ,
P O W E R P C ,
} ;
c o n s t f l o a t P I = 3. 1 4 1 5 9 ; / * O K * /
t y p e d e f i n t a g e ; / * C - V 1 v i o l a t i o n * /
t y p e d e f s t r u c t i n t _ c o u p l e p i x e l _ t ; / * O K * /

```
Abbreviations are tolerated as long as they significantly reduce the name length without
losing meaning.
```
### C-V2 - Structures

Variables can be grouped together into a structure if and only if they form **a coherent entity**.
Structures must be kept **as small as possible**.
s t r u c t p e r s o n { / * O K * /
c h a r * n a m e ;
u n s i g n e d i n t a g e ;
f l o a t s a l a r y ;
} ;

s t r u c t d a t a { / * C - V 2 v i o l a t i o n * /
s t r u c t p e r s o n p l a y e r ;
u n s i g n e d i n t w i d t h ;
u n s i g n e d i n t l e n g t h ;
u n s i g n e d i n t s c o r e ;
i n t i ;
} ;


### C-V3 - Pointers

The asterisk (*) must be **attached to the associated variable** , with no spaces in between.

It must also be **preceded by a space** , except when it is itself preceded by another asterisk.

This includes using the asterisk to declare or dereference a pointer.

When used in a cast, the asterisk must have a space on its left side, but not on its right side.

```
OK C-V3 violation
int *a; int* a;
char **argv; char**argv;
char *const *array; char * const *array;
int a = 3 * b; int a = 3*b;
int strlen(char const *str); int strlen(char const* str);
char *nbr_to_str(int i); char* nbr_to_str(int i);
my_put_nbr(*ptr); my_put_nbr(* ptr);
(int *) ptr; (int*) ptr;as well as(int * ) ptr;
void (*func_ptr)(int)= &func; void (* func_ptr)(int)= &func;
```
```
This rule applies only in the pointer context.
```

## C-C - Control structures

Unless otherwise specified, all control structures are allowed.

### C-C1 - Conditional branching

Anifconditional block (along withelse ifandelse) **must not contain more than 3 branches**.

```
Arrays of function pointers andswitchinstructions are very useful when you want to have
numerous different behaviours that can result from the check of an element.
Make sure to choose the most suitable one.
```
**Nested conditional branches** with a depth of 3 or more must be avoided.

```
If you need multiple levels of branches, you probably need to refactor your function into
sub-functions.
```
i f (... ) { / * O K * /
d o _ s o m e t h i n g ( ) ;
} e l s e i f (... ) {
d o _ s o m e t h i n g _ e l s e ( ) ;
} e l s e {
d o _ s o m e t h i n g _ m o r e ( ) ;
}

i f (... ) {
d o _ s o m e t h i n g ( ) ;
} e l s e i f (... ) {
d o _ s o m e t h i n g _ e l s e ( ) ;
} e l s e i f (... ) {
d o _ s o m e t h i n g _ m o r e ( ) ;
} e l s e { / * C - C 1 v i o l a t i o n * /
d o _ o n e _ l a s t _ t h i n g ( ) ;
}

w h i l e (... ) { / * O K * /
i f (... ) {
d o _ s o m e t h i n g ( ) ;
}
}


w h i l e (... ) { / * C - C 1 v i o l a t i o n * /
f o r (... ) {
i f (... ) {
d o _ s o m e t h i n g ( )
}
}
}

```
else ifbranching does not add one, but two levels of depth, as it is considered to be anif
inside anelse.
```
/ / T h i s c o d e...
i f (... ) {
d o _ s o m e t h i n g ( ) ;
} e l s e i f (... ) {
d o _ s o m e t h i n g _ e l s e ( ) ;
} e l s e i f (... ) { / * C - C 1 v i o l a t i o n * /
d o _ o n e _ l a s t _ t h i n g ( ) ;
}

/ /... i s u n f o l d e d a s t h i s
i f (... ) {
d o _ s o m e t h i n g ( ) ;
} e l s e {
i f (... ) {
d o _ s o m e t h i n g _ e l s e ( ) ;
} e l s e {
i f (... ) { / * C - C 1 v i o l a t i o n * /
d o _ o n e _ l a s t _ t h i n g ( ) ;
}
}
}

/ / T h i s c o d e...
i f (... ) {
d o _ s o m e t h i n g ( ) ;
} e l s e i f (... ) {
w h i l e (... ) { / * C - C 1 v i o l a t i o n * /
d o _ s o m e t h i n g _ e l s e ( ) ;
}
}

/ /... i s u n f o l d e d a s t h i s
i f (... ) {
d o _ s o m e t h i n g ( ) ;
} e l s e {
i f (... ) {
w h i l e (... ) { / * C - C 1 v i o l a t i o n * /
d o _ s o m e t h i n g _ e l s e ( ) ;
}
}
}


### C-C2 - Ternary operators

The use of ternary operators is **allowed as far as it is kept simple and readable** , and if it does
not obfuscate code.

```
You must never use nested or chained ternary operators.
You must always use the value produced by a ternary operator (by assigning it to a vari-
able or returning it for example).
```
p a r i t y _ t y e a r _ p a r i t y = ( y e a r % 2 = = 0 )? E V E N : O D D ; / * O K * /
r e t u r n ( a > 0? a : 0 ) ; / * O K * /
u n s i g n e d i n t s a f e _ s u m = i s _ s u m _ o v e r f l o w ( a , b )? 0 : a + b ; / * O K * /
c h a r * r e s u l t = i s _ c o r r e c t l y _ f o r m a t t e d ( s t r )? s t r : f o r m a t ( s t r ) ; / * O K * /
i n t a = b > 1 0? c < 2 0? 5 0 : 8 0 : e = = 2? 4 : 8 ; / * C - C 2 v i o l a t i o n * /
a l r e a d y _ c h e c k e d? g o _ t h e r e ( ) : c h e c k ( ) ; / * C - C 2 v i o l a t i o n * /
f i r s t ( )? s e c o n d ( ) : 0 ; / * C - C 2 v i o l a t i o n * /

### C-C3 - goto

Using thegoto **keyword is forbidden** , because it can very quickly participate in the creation of
infamous spaghetti code, which is completely unreadable.


## C-H - Header files

### C-H1 - Content

Header files must only contain:

```
✓ function prototypes ,
✓ type declarations ,
✓ structure declarations ,
✓ enumeration declarations ,
✓ global variable/constant declarations ,
✓ macros ,
✓ static inline functions.
```
All these elements must **only** be found in header files, and thus not in source files.

```
Including a header from another header is allowed as long as the header file itself needs
it.
If a source file requires it, but not the header file itself, it should then be included in the
source file instead.
```
```
In order to keep your code simple and readable, you should not use conditional prepro-
cessor directives in source files.
```
### C-H2 - Include guard

Headers must be protected from **double inclusion** , by using the#ifndef,#define, and#endifpre-
processor directives.


### C-H3 - Macros

Macros must match **only one statement** , and **fit on a single line**.

# d e f i n e P I 3. 1 4 1 5 9 2 6 5 3 5 8 9 7 9 3 2 3 8 4 6 / * O K * /
# d e f i n e D E L T A ( a , b , c ) ( ( b ) * ( b ) - 4 * ( a ) * ( c ) ) / * O K * /
# d e f i n e P R I N T _ N E X T ( n u m ) { n u m + + ; p r i n t f ( " % d " , n u m ) ; } / * C - H 3 v i o l a t i o n * /
# d e f i n e E R R O R _ M E S S A G E " M u l t i l i n e m a c r o s " \ / * C - G 6 v i o l a t i o n * /
" h a v e t o b e a v o i d e d " / * C - H 3 v i o l a t i o n * /

```
Using a macro to shorten a long expression is not a valid reason to use a macro:
/ / U n n e c e s s a r y a n d o b f u s c a t e s t h e c o d e
# d e f i n e W I N ( d a t a - > o b j e c t - > s c e n e - > s t a t e - > w i n d o w )
```

## C-A - Advanced

### C-A1 - Constant pointers

When creating a pointer, if the pointed data is not (or should not be) modified by the function,
it should be marked as **constant** (const).

### C-A2 - Typing

Prefer the **most accurate types possible** according to the use of the data.

i n t c o u n t e r ; / * C - A 2 v i o l a t i o n * /
u n s i g n e d i n t c o u n t e r ; / * O K * /
u n s i g n e d i n t g e t _ o b j _ s i z e ( v o i d c o n s t * o b j e c t ) / * C - A 2 v i o l a t i o n * /
s i z e _ t g e t _ o b j _ s i z e ( v o i d c o n s t * o b j e c t ) / * O K * /

```
Useful types includesize_t,ptrdiff_t,uint8_t,int32_t, and more...
```
### C-A3 - Line break at the end of file

Files must **end with a line break**.

### ∇ Terminal - + x

```
$> cat -e correct.c
int main(void) {$
return 0;$
}$
$> cat -e incorrect.c
int main(void) {$
return 0;$
}
```
The reason for this is tied to the POSIX’s definition of a line:

```
A sequence of zero or more non-<newline> characters plus a terminating <newline>
character.
— The Open Group Base Specifications Issue 7, 2018 edition —
```

### C-A4 - static

Global variables and functions that are not used outside the compilation unit to which they be-
long should be **marked with the** static **keyword**.

```
Be careful not to confuse the different uses of thestatickeyword.
It does very different things depending on where you use it.
```

v 7


