#include "cli.h"
#include "usart.h"
#include <string.h>
#include <ctype.h>

// head of CLI command list
static CLI_CommandItem *head = NULL;

// char buffer where command will be stored
static char commandBuffer[100];

/**
 * This function searches the CLI command list and tries to find a descriptor for the provided command.
 * The command format is case-insensitive.
 * Returns pointer to @ref CLI_MenuItem structure if given command was found in the CLI command list.
 *
 * @param command pointer to command (string)
 *
 * @retval pointer to @ref CLI_MenuItem structure desrcibing the command, if command was found
 * @retval NULL if the given command does not match any command regitstered in the CLI command list 
 */
static CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command);

/**
 * @bref This function is responsible for collecting the command reading in from USART.
 *
 * This function should check wether the USART interface has some new data. If it does
 * this function reads new characters and stores them in a command buffer. This function
 * is also responsible for providing echo of the input characters back to the buffer.
 *
 * The function exits when:
 * - no more characters are available to read from USART - the function returns false
 * - new line was detected - this function returns true
 *
 * @retval true if command was collected
 * @retval false if command is yet incomplete
 */
static bool CLI_StoreCommand(void);

/**
 * @brief This function converts string to a lowercase
 *
 * @param dst pointer where converted null terminated string will be stored
 * @param src pointer to string which will be converted
 */
static void CLI_StringToLower(char *dst, const char *src);
	
	
	
void CLI_Proc(void){
	if(CLI_StoreCommand()){
		//char lowerBuffer[100];
		//CLI_StringToLower(lowerBuffer, commandBuffer);
		//CLI_CommandItem *item = CLI_GetMenuItemByCommandName(lowerBuffer);
		CLI_CommandItem *item = CLI_GetMenuItemByCommandName(commandBuffer);
		if(item!=NULL){
			item->callback(NULL);
		} else {
			USART_WriteString(commandBuffer);
			USART_WriteString(": command unknown. Use '?' to get a list of known commands.\n\r");
		}
		memset(commandBuffer, 0, sizeof(commandBuffer));
	}
}

bool CLI_AddCommand(CLI_CommandItem *item){
	if(item->callback != NULL && strlen(item->commandName)>0){
		item->next = head;
		head = item;
		return true;
	}
	return false;
}

void CLI_PrintAllCommands(void){
	CLI_CommandItem *item = head;
	while(item!=NULL){
		USART_WriteString(item->commandName);
		USART_WriteString(": ");
		if(item->description!=NULL){
			USART_WriteString(item->description);
		} else {
			USART_WriteString("(no description)");
		}
		USART_PutChar('\n');
		item=item->next;
	}
}

CLI_CommandItem* CLI_GetMenuItemByCommandName(char *command){
	CLI_CommandItem *item = head;
	while(item!=NULL && strcasecmp(item->commandName, command)){
		item=item->next;
	}
	return item;
};

void CLI_StringToLower(char *dst, const char *src){
	while(src){
		*(dst++)=tolower(*(src++));
	}
}

bool CLI_StoreCommand(){
	char c;
	if(USART_GetChar(&c)){
		USART_PutChar(c);
		if(c=='\n' || c=='\r'){
			//USART_WriteString("\n\r");
			return true;
		} else {
			//USART_PutChar(c);
			strncat(commandBuffer, &c, 1);
		}
	}
	
	return false;
}
