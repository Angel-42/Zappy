##
## EPITECH PROJECT, 2025
## Zappy
## File description:
## Main Makefile
##

# Binary names
SERVER_NAME = zappy_server
AI_NAME = zappy_ai
GUI_NAME = zappy_gui

# Directories
NETWORK_DIR = src/Network
GUI_DIR = src/GUI
AI_DIR = src/AI

# Build commands
MAKE_CMD = make
BUILD_CMD = ./Build.sh

all: server ai gui

server:
	@echo "Building server..."
	@$(MAKE_CMD) -C $(NETWORK_DIR)
	@cp $(NETWORK_DIR)/$(SERVER_NAME) ./$(SERVER_NAME)
	@echo "Server built successfully: $(SERVER_NAME)"

ai:
	@echo "Building AI client..."
	@cd $(AI_DIR) && $(BUILD_CMD)
	@cp $(AI_DIR)/build/$(AI_NAME) ./$(AI_NAME)
	@echo "AI client built successfully: $(AI_NAME)"

gui:
	@echo "Building GUI client..."
	@cd $(GUI_DIR) && $(BUILD_CMD)
	@cp $(GUI_DIR)/build/$(GUI_NAME) ./$(GUI_NAME)
	@echo "GUI client built successfully: $(GUI_NAME)"

clean:
	@echo "Cleaning server..."
	@$(MAKE_CMD) -C $(NETWORK_DIR) clean
	@echo "Cleaning AI client..."
	@cd $(AI_DIR) && $(BUILD_CMD) clean 2>/dev/null || true
	@echo "Cleaning GUI client..."
	@cd $(GUI_DIR) && $(BUILD_CMD) clean 2>/dev/null || true

fclean: clean
	@echo "Full cleaning..."
	@$(MAKE_CMD) -C $(NETWORK_DIR) fclean
	@$(RM) $(SERVER_NAME)
	@$(RM) $(AI_NAME)
	@$(RM) $(GUI_NAME)
	@echo "Full clean completed"

re: fclean all

tests_run:
	@echo "Running tests..."
	@$(MAKE_CMD) -C $(NETWORK_DIR) tests_run 2>/dev/null || \
	echo "No tests available for server"

.PHONY: all server ai gui clean fclean re tests_run
