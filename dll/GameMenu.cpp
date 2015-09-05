#include "GameMenu.h"

const char * GameMenu::gameMenuFile = "cstrike/resource/GameMenu.res";

const char * GameMenu::fileContextFirst = "\"GameMenu\"\
{\n\
\t\"1\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_ResumeGame\"\n\
\t\t\"command\" \"ResumeGame\"\n\
\t\t\"OnlyInGame\" \"1\"\n\
\t}\n\
\t\"2\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_Disconnect\"\n\
\t\t\"command\" \"Disconnect\"\n\
\t\t\"OnlyInGame\" \"1\"\n\
\t\t\"notsingle\" \"1\"\n\
\t}\n\
\t\"4\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_PlayerList\"\n\
\t\t\"command\" \"OpenPlayerListDialog\"\n\
\t\t\"OnlyInGame\" \"1\"\n\
\t\t\"notsingle\" \"1\"\n\
\t}\
\t\"5\"\n\
\t{\n\
\t\t\"label\" \"\"\n\
\t\t\"command\" \"\"\n\
\t\t\"OnlyInGame\" \"0\"\n\
\t}\n\
";

const char * GameMenu::fileContextSecond = "\n\t\"19\"\n\
\t{\n\
\t\t\"label\" \"\"\n\
\t\t\"command\" \"\"\n\
\t\t\"OnlyInGame\" \"0\"\n\
\t}\n\
\t\"20\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_NewGame\"\n\
\t\t\"command\" \"OpenCreateMultiplayerGameDialog\"\n\
\t}\n\
\t\"21\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_FindServers\"\n\
\t\t\"command\" \"OpenServerBrowser\"\n\
\t}\n\
\t\"22\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_Options\"\n\
\t\t\"command\" \"OpenOptionsDialog\"\n\
\t}\n\
\t\"23\"\n\
\t{\n\
\t\t\"label\" \"#GameUI_GameMenu_Quit\"\n\
\t\t\"command\" \"Quit\"\n\
\t}\n\
}";

void GameMenu::saveServers(  std::vector< std::unique_ptr< Server > > & servers ){

    if( servers.size() == 0 ){
        return;
    }

    checkAndSetPermissions( GameMenu::gameMenuFile , false , false , false );

    FILE * fileGameMenu = fopen( GameMenu::gameMenuFile , "wt" );

    if( !fileGameMenu ){
        return;
    }

    fputs( GameMenu::fileContextFirst , fileGameMenu );

    int currentNumber = 6;

    for( std::vector< std::unique_ptr< Server > >::iterator it = servers.begin() ; it != servers.end() ; ++it ){
        char buffer[ 512 ];
        snprintf( buffer, sizeof( buffer ) - 1 ,"\n\t\"%d\"\n\
\t{\n\
\t\t\"label\" \"%s\"\n\
\t\t\"command\" \"engine connect %s\"\n\
\t}" , currentNumber , (*it) -> getName().c_str() , ( *it ) -> getIp().c_str() );

        fputs( buffer , fileGameMenu );

        currentNumber++;
    }

    fputs( GameMenu::fileContextSecond , fileGameMenu );

    fclose( fileGameMenu );

    checkAndSetPermissions( GameMenu::gameMenuFile , true , true , true );
}
