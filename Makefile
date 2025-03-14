#*******************************  VARIABLES  **********************************#

NAME			=	MattDaemon

# --------------- FILES --------------- #

include src.mk

# ------------ DIRECTORIES ------------ #

DIR_BUILD		=	.build/
DIR_SRC 		=	src/
DIR_INCLUDE		=	include/
DIR_INCLUDES	=	$(DIR_INCLUDE)

# ------------- SHORTCUTS ------------- #

OBJ				=	$(patsubst %.cpp, $(DIR_BUILD)%.o, $(SRC))
DEP				=	$(patsubst %.cpp, $(DIR_BUILD)%.d, $(SRC))
SRC				=	$(addprefix $(DIR_SRC),$(LIST_SRC))

INCLUDE			=	$(shell find $(DIR_INCLUDE) -type f -name "*.hpp")
INCLUDES		=	$(addprefix -I , $(DIR_INCLUDES))

CLANG_FORMAT	=	clang-format-15
CPPCHECK		=	cppcheck


# ------------ COMPILATION ------------ #

CPPFLAGS		=	-Wall -Wextra -Werror -std=c++11

DEP_FLAGS		=	-MMD -MP

# -------------  COMMANDS ------------- #

RM				=	rm -rf
MKDIR			=	mkdir -p

#***********************************  RULES  **********************************#

.PHONY: all
all:			$(NAME)

# ---------- VARIABLES RULES ---------- #

$(NAME):		$(OBJ)
				$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJ)

# ---------- COMPILED RULES ----------- #

-include $(DEP)

$(DIR_BUILD)%.o: %.cpp
				mkdir -p $(shell dirname $@)
				$(CXX) $(CPPFLAGS) $(DEP_FLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
				$(RM) $(DIR_BUILD)

.PHONY: fclean
fclean: clean
				$(RM) $(NAME)

.PHONY: re
re:				fclean
				$(MAKE) all

.PHONY: check-format
check-format:
				$(CLANG_FORMAT) -style=file $(SRC) $(INCLUDE) -n --Werror

.PHONY: format
format:
				$(CLANG_FORMAT) -style=file $(SRC) $(INCLUDE) -i
