
#include "windows.h"
#include "ddraw.h"
#include "dinput.h"




bool InitSurf(); // Initialisation des surfaces directdraw
bool InitDDObject(HWND);	// initialise l'objet Direct Draw (mode exclusif, résolution)
bool Update();	 // mise a jour de l'image
void ExitDD();
int correction_hasard();
int deplacement_tie1();
int deplacement_tie2();
int deplacement_tie3();
int deplacement_imp1();
int deplacement_imp2();
int deplacement_imp3();



int phase_1();
int phase_2();
int phase_3();
int phase_4();
int phase_4neg();
int phase_5();
int phase_6();
int phase_7();
int phase_8();
int phase_9();
int phase_3spec();

IDirectDrawSurface * CreateOffScreenSurface(IDirectDraw *,int,int);
HRESULT DDCopyBitmap(IDirectDrawSurface *,HBITMAP, int,int);

/*********** Variables globales *************/

LPDIRECTDRAW lpDD=NULL;
LPDIRECTDRAWSURFACE lpDDSPrim=NULL;	// Surface Primaire
LPDIRECTDRAWSURFACE lpDDSBack=NULL;	// Back
LPDIRECTDRAWSURFACE lpDDSLogoAmanga=NULL; 
LPDIRECTDRAWSURFACE lpDDSmenu_principal=NULL; 
LPDIRECTDRAWSURFACE lpDDSphase3=NULL;
LPDIRECTDRAWSURFACE lpDDSperdu_map1=NULL; 
LPDIRECTDRAWSURFACE lpDDSgagne_map1=NULL;
LPDIRECTDRAWSURFACE lpDDSselect=NULL; 
LPDIRECTDRAWSURFACE lpDDSforce=NULL; 
LPDIRECTDRAWSURFACE lpDDSfond_map1=NULL; 
LPDIRECTDRAWSURFACE lpDDSfond_map2=NULL;
LPDIRECTDRAWSURFACE lpDDSfond_map3=NULL;
LPDIRECTDRAWSURFACE lpDDScalque_vaisseau_map2=NULL; 
LPDIRECTDRAWSURFACE lpDDScalque_map3=NULL; 
LPDIRECTDRAWSURFACE lpDDSxwing=NULL;
LPDIRECTDRAWSURFACE lpDDSluke=NULL;
LPDIRECTDRAWSURFACE lpDDSdv=NULL;
LPDIRECTDRAWSURFACE lpDDSpasserelle=NULL;
LPDIRECTDRAWSURFACE lpDDSimp=NULL;
LPDIRECTDRAWSURFACE lpDDStie=NULL;
LPDIRECTDRAWSURFACE lpDDSporte=NULL;
LPDIRECTDRAWSURFACE lpDDScalque_sortie_map1=NULL;
LPDIRECTDRAWSURFACE lpDDSled_map1=NULL; 
LPDIRECTDRAWSURFACE lpDDSled_map2=NULL; 
LPDIRECTDRAWSURFACE lpDDStexte=NULL; 
LPDIRECTDRAWSURFACE lpDDSmenu_de_jeu=NULL; 



short niveau=1;
int hasard=1;
int phase_de_jeu=1, menu_jeu;
int pos_x=0;
int pos_y=100;
int selection=1;
int compteur_led=0;
double force=50;
int x_tie1, y_tie1, x_tie2, y_tie2, x_tie3, y_tie3, x_imp1, y_imp1, x_imp2, y_imp2, x_imp3, y_imp3, x_dv, y_dv;
int tour_tie1, tour_tie2, tour_tie3, tour_imp1, tour_imp2, tour_imp3;
int tour_cinematique=1;
int direction, direction_tie1, direction_tie2, direction_tie3, direction_imp1, direction_imp2, direction_imp3, direction_dv;
int tour=1, tour_led_map1, tour_position=1, tour_position_imp1=1, tour_position_imp2, tour_position_imp3, tour_position_dv;

/**************** Fonctions ****************/

IDirectDrawSurface * DDLoadBitmap(IDirectDraw *pdd, LPCSTR szBitmap)
{
	HBITMAP hbm;
	BITMAP bm;
	IDirectDrawSurface *pdds;
	
	hbm = (HBITMAP)LoadImage(NULL, szBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);
	if (hbm == NULL)
		return NULL;

	GetObject(hbm, sizeof(bm), &bm); // remplit la structure BITMAP

	pdds = CreateOffScreenSurface(pdd, bm.bmWidth, bm.bmHeight);
	if (pdds)
		DDCopyBitmap(pdds, hbm, bm.bmWidth, bm.bmHeight);

	DeleteObject(hbm);
	return pdds;
}

IDirectDrawSurface * CreateOffScreenSurface(IDirectDraw *pdd, int dx, int dy)
{
	DDSURFACEDESC ddsd;
	IDirectDrawSurface *pdds;
	
	// Remplissage de la structure
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT |DDSD_WIDTH|DDSD_CKSRCBLT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = dx;
	ddsd.dwHeight = dy;
	ddsd.ddckCKSrcBlt.dwColorSpaceLowValue = RGB(0,255,0);
	ddsd.ddckCKSrcBlt.dwColorSpaceHighValue = RGB(0,255,0);

	
	// Création de la surface
	if (pdd->CreateSurface(&ddsd, &pdds, NULL) != DD_OK)
		return NULL;
	else 
		return pdds;
}

HRESULT DDCopyBitmap(IDirectDrawSurface *pdds, HBITMAP hbm, int dx, int dy)
{
	HDC hdcImage;
	HDC hdc;
	HRESULT hr;
	HBITMAP hbmOld;

	hdcImage = CreateCompatibleDC(NULL);
	hbmOld = (HBITMAP)SelectObject(hdcImage, hbm);

	if ((hr = pdds->GetDC(&hdc)) == DD_OK)
	{
		BitBlt(hdc, 0, 0, dx, dy, hdcImage, 0, 0, SRCCOPY);
		pdds->ReleaseDC(hdc);
	}

	SelectObject(hdcImage, hbmOld);
	DeleteDC(hdcImage);
	return hr;
}

int DDText(LPDIRECTDRAWSURFACE lpdds, char* text, int x, int y, COLORREF color)
{
	// Récupération du contexte graphique
	HDC hdc;
	if(FAILED(lpdds->GetDC(&hdc)))
	{
		return(0);
	}

	// Définition de la couleur du texte et du fond (transparent)
	SetTextColor(hdc, color);
	SetBkMode(hdc, TRANSPARENT);

	// Affichage du texte
	TextOut(hdc, x, y, text, strlen(text));

	// Libération du contexte graphique
	lpdds->ReleaseDC(hdc);

	return(1);
}



long FAR PASCAL WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		ExitDD();
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
			case VK_ESCAPE:
				if(phase_de_jeu==4||phase_de_jeu==6||phase_de_jeu==8)
                {menu_jeu=1;}
			    
			    
		}
		switch(wParam)
		{
			case VK_MULTIPLY:
				if(menu_jeu==4)phase_de_jeu++;
			    
			    
		}
		switch(wParam)
		{
			case VK_DIVIDE:
				if(menu_jeu==4)phase_de_jeu-=2;
			    
			    
		}
		switch(wParam)
		{
			case VK_END:
				if(menu_jeu==1)niveau++;
			    
			    
		}
		switch(wParam)
		{
			case VK_PAUSE:
				if(menu_jeu==1)niveau--;
			    
			    
		}

        switch(wParam)
		{
			case VK_HOME:
				if(phase_de_jeu==6)
                {if(pos_x>140&&pos_x<160&&pos_y>340&&pos_y<360){
                phase_de_jeu=-3;
                 direction=1;
            pos_x=50;
            pos_y=210;
            tour_led_map1=1;
            x_tie1=290;
            y_tie1=420;
            x_tie2=500;
            y_tie2=120;
            x_tie3=500;
            y_tie3=210;
            tour_tie1=1;
            tour_tie2=1;
            tour_tie3=1;
            tour=1;}}
			    
			    
		}
        switch(wParam)
		{
			case VK_DOWN:
				if(phase_de_jeu==2){selection++;
			    if(selection>2)selection=1;}
			    
                
                if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
			    {menu_jeu++;
                if(menu_jeu>4)menu_jeu=1;
                }
                else{
                
                
                pos_y=pos_y+4;
			    direction=3;
			    tour_position++;
			    if(tour_position>6)tour_position=1;
			    }
                hasard=hasard+4;
			    
		}
		switch(wParam)
		{
			case VK_UP:
				if(phase_de_jeu==2){selection--;
			    if(selection<1)selection=2;}
			    
                
                if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
			    {menu_jeu--;
                if(menu_jeu<1)menu_jeu=4;
                }
                
                else{
		        pos_y=pos_y-4;
		        direction=1;
		        tour_position++;
			    if(tour_position>6)tour_position=1;}
		        hasard=hasard-7;
		        
        }
		switch(wParam)
		{
			case VK_LEFT:
			if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
			    {}
			    else{
				pos_x=pos_x-4;	
                direction=4;
                tour_position++;
			    if(tour_position>6)tour_position=1;}
                hasard=hasard+1;		
		}
		switch(wParam)
		{
			case VK_RIGHT:
			if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
			    {}
			    else{
				pos_x=pos_x+4;
				direction=2;
				tour_position++;
			    if(tour_position>6)tour_position=1;}
				hasard=hasard-3;
		}
		switch(wParam)
		{
			case VK_SPACE:
			if(phase_de_jeu==8)
			    {if(force>=10){
			    
			    if(direction==1)
                {
                if(pos_y-y_dv<50&&pos_y-y_dv>0&&pos_x-x_dv<30&&pos_x-x_dv>-30)
                y_dv-=40;
                }
                
                if(direction==2)
                {
                if(x_dv-pos_x<50&&x_dv-pos_x>0&&y_dv-pos_y<30&&y_dv-pos_y>-30)
                x_dv+=40;
                }
                
			    if(direction==3)
                {
                if(y_dv-pos_y<50&&y_dv-pos_y>0&&x_dv-pos_x<30&&x_dv-pos_x>-30)
                y_dv+=40;
                }
                
                if(direction==4)
                {
                if(pos_x-x_dv<50&&pos_x-x_dv>0&&pos_y-y_dv<30&&pos_y-y_dv>-30)
                x_dv-=40;
                }
                
                
                force-=10;}}
			    
		}
		switch(wParam)
		{
			case VK_RETURN:
			if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
			    {if(menu_jeu==1)
			    {menu_jeu=-9;}
                if(menu_jeu==2)
			    {phase_de_jeu=3;niveau=1;
                menu_jeu=-9;}
			    if(menu_jeu==3)
			    {phase_de_jeu=2;
                menu_jeu=-9;}
			    if(menu_jeu==4)
			    {PostQuitMessage(0);
		        ExitDD();}
		        }
		        else{
		        
		        if(phase_de_jeu==-4){ phase_de_jeu=3; break;}
		        
		        if(phase_de_jeu==1) {phase_de_jeu=2; break;}
		        
		        if(phase_de_jeu==2&&selection==1){ phase_de_jeu=3; break;}
				if(phase_de_jeu==2&&selection==2){PostQuitMessage(0); ExitDD();}
				
				if(phase_de_jeu==3){ phase_de_jeu=4; break;}
				
				if(phase_de_jeu==5){ phase_de_jeu=6; break;}
				
				if(phase_de_jeu==7){ phase_de_jeu=8; break;}
				
		        if(phase_de_jeu==9&&tour_cinematique==2){if(niveau==2) niveau=3; if(niveau==1) niveau=2;  phase_de_jeu=3; break;}
		        if(phase_de_jeu==9&&tour_cinematique==1) {tour_cinematique=2; break;}
                
				
                			
				
		        
                
                }
                
				
			
		}
		break;

		
	case WM_CLOSE:
		ExitDD();
		DestroyWindow(hwnd);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}



int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpszCmpParam,int nCmdShow)
{
	WNDCLASS W;
	HWND hwnd;
	LPSTR Name="-- S-W 2 --";
	MSG msg;

	memset(&W,0,sizeof(WNDCLASS));

	W.style=CS_HREDRAW | CS_VREDRAW;
	W.hInstance=hInst;
	W.lpszClassName=Name;
	W.hbrBackground=(HBRUSH) GetStockObject(BLACK_BRUSH);
	W.cbClsExtra=0;
	W.cbWndExtra=0;
	W.lpfnWndProc=WndProc;

	RegisterClass(&W);

	hwnd=CreateWindowEx(0,Name,Name,WS_POPUP,
			0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL,NULL,hInst,NULL);

	ShowCursor(false);

	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);

	InitDDObject(hwnd);
	InitSurf();




	while(1)	// Boucle infinie
    {
	if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}

    }
    return msg.wParam;
}

bool InitDDObject(HWND hwnd)
{
	HRESULT ddrval;
	ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
	if( ddrval != DD_OK )
	{
		return false;
	}
                
	ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
	if( ddrval != DD_OK )
	{
		lpDD->Release();
		return false;
	}

	ddrval = lpDD->SetDisplayMode(640,480,32);
	if( ddrval != DD_OK )
	{
		lpDD->Release();
		return false;
	}
	return true;
}


bool InitSurf()
{
	DDSURFACEDESC desc; // Descripteur de surface
	HRESULT ddrval;	// utilisé pour récupérer les erreurs
	DDSCAPS ddscaps;
	
	// creation du descripteur de la surface primaire
	ZeroMemory(&desc,sizeof(desc));
	desc.dwSize=sizeof(desc);
	desc.dwFlags=DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    desc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
	desc.dwBackBufferCount = 1;


	// on crée la surface à partir du descripteur
	ddrval=lpDD->CreateSurface(&desc,&lpDDSPrim,NULL);
	if(ddrval!=DD_OK)
		exit(0);

	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddrval = lpDDSPrim->GetAttachedSurface(&ddscaps, &lpDDSBack);
    if( ddrval != DD_OK )
    {
        lpDDSPrim->Release();
        lpDD->Release();
        return(false);
    }


	lpDDSLogoAmanga=DDLoadBitmap(lpDD,"data/logo.bmp");
	lpDDSmenu_principal=DDLoadBitmap(lpDD,"data/menu_principal.bmp");

    lpDDSselect=DDLoadBitmap(lpDD,"data/select.bmp");
    lpDDSpasserelle=DDLoadBitmap(lpDD,"data/passerelle_map3.bmp");
   	lpDDSfond_map1=DDLoadBitmap(lpDD,"data/fond_map1.bmp");
   	lpDDSfond_map2=DDLoadBitmap(lpDD,"data/fond_map2.bmp");
   	lpDDSfond_map3=DDLoadBitmap(lpDD,"data/fond_map3.bmp");
   	lpDDScalque_vaisseau_map2=DDLoadBitmap(lpDD,"data/calque_vaisseau_map2.bmp");
   	lpDDScalque_map3=DDLoadBitmap(lpDD,"data/calque_map3.bmp");
   	lpDDSled_map1=DDLoadBitmap(lpDD,"data/led_map1.bmp");
   	lpDDSled_map2=DDLoadBitmap(lpDD,"data/detecte.bmp");
   	lpDDSforce=DDLoadBitmap(lpDD,"data/force.bmp");
   	
	return true ;
}

bool Update()
{
	
if(phase_de_jeu==1) phase_1();

if(phase_de_jeu==2) phase_2();

if(phase_de_jeu==3) phase_3();

if(phase_de_jeu==-3) phase_3spec();

if(phase_de_jeu==4) phase_4();

if(phase_de_jeu==-4) phase_4neg();

if(phase_de_jeu==5) phase_5(); 

if(phase_de_jeu==6) phase_6();

if(phase_de_jeu==7) phase_7();

if(phase_de_jeu==8) phase_8();

if(phase_de_jeu==9) phase_9();
 
	return true;
}

int phase_1(void)
{
	RECT rc;
	
	SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSLogoAmanga, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
	

	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);

return 0;}

int phase_2(void)
{
    RECT rc;
	
	SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSmenu_principal, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;


  if(selection==1){	SetRect(&rc,0,0,17,15);
	                if (FAILED (lpDDSBack->BltFast (332, 361, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                	return FALSE;
	
                     SetRect(&rc,0,0,17,15);
                     if (FAILED (lpDDSBack->BltFast (615, 361, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                     return FALSE;}

  if(selection==2){	SetRect(&rc,0,0,17,15);
	                if (FAILED (lpDDSBack->BltFast (395, 422, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                	return FALSE;
	
                     SetRect(&rc,0,0,17,15);
                     if (FAILED (lpDDSBack->BltFast (538, 422, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                     return FALSE;}


	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);
 
    return 0;}


int phase_3(void)
{
    direction=1;
            pos_x=50;
            pos_y=210;
            tour_led_map1=1;
            x_tie1=290;
            y_tie1=420;
            x_tie2=500;
            y_tie2=120;
            x_tie3=500;
            y_tie3=210;
            tour_tie1=1;
            tour_tie2=1;
            tour_tie3=1;
            tour=1;
     
           	if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}      
             lpDDStexte=DDLoadBitmap(lpDD,"data/fond_texte.bmp");
	DDText(lpDDStexte, "La Rebellion vit des heures sombres. En effet, l'Empire galactique, déjà au comble", 10, 30, RGB(255,255,0));
	DDText(lpDDStexte, "de sa puissance avec son Etoile Noire, vient de dérober les plans d'un tout nouveau", 10, 60, RGB(255,255,0));
	DDText(lpDDStexte, "vaisseau de combat, le V-Wing, un prototype top secret qui aurait peut-être permit", 10, 90, RGB(255,255,0));
	DDText(lpDDStexte, "à la Rebellion de vaincre l'Empire. ", 10, 120, RGB(255,255,0));
	DDText(lpDDStexte, "Heuresement, les plans ont été codés, et l'Empire n'a pas encore réussi à étudier les", 10, 150, RGB(255,255,0));
	DDText(lpDDStexte, "plans du vaisseau, ce qui annoncerait la ruine définitive des Rebelles.", 10, 180, RGB(255,255,0));
	DDText(lpDDStexte, "Votre mission est de vous infiltrer dans l'Etoile Noire afin d'y dérober les plans", 10, 220, RGB(255,255,0));
	DDText(lpDDStexte, "secrets et de les ramener au plus vite.", 10, 250, RGB(255,255,0));
	DDText(lpDDStexte, "Aux commandes de votre X-Wing, évitez les chasseurs Tie ennemis et les sondes", 10, 290, RGB(255,255,0));
	DDText(lpDDStexte, "pour que votre mission reste inconnue de l'ennemi le plus longtemps possible.", 10, 320, RGB(255,255,0));
	DDText(lpDDStexte, "Que la force soit avec vous !", 300, 370, RGB(255,255,0));
            
            RECT rc;
            
	SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDStexte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
	

	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);
return 0;}


int phase_4(void)
{



correction_hasard();

    RECT rc;

    if(pos_x<50)    pos_x=50;
    if(pos_x>500&&pos_y<175)   pos_x=500;
    if(pos_x>500&&pos_y>245)   pos_x=500;
    if(pos_y<30)    pos_y=30;
    if(pos_y>420)   pos_y=420;
    if(pos_x>50&&pos_x<60&&pos_y>34&&pos_y<266) pos_x=50;
    if(pos_x>50&&pos_x<60&&pos_y>274&&pos_y<416) pos_x=50;
	if(pos_x>54&&pos_x<106&&pos_y>270&&pos_y<275) pos_y=270;
	if(pos_x>54&&pos_x<106&&pos_y>265&&pos_y<270) pos_y=270;
	if(pos_x>54&&pos_x<106&&pos_y>414&&pos_y<420) pos_y=420;
    if(pos_x>54&&pos_x<224&&pos_y>30&&pos_y<38) pos_y=30;
    if(pos_x>104&&pos_x<110&&pos_y>274&&pos_y<416) pos_x=110;
    if(pos_x>104&&pos_x<110&&pos_y>34&&pos_y<266) pos_x=110;
    if(pos_x>54&&pos_x<224&&pos_y>84&&pos_y<90) pos_y=90;
    if(pos_x>114&&pos_x<120&&pos_y>94&&pos_y<176) pos_x=114;
    if(pos_x>114&&pos_x<166&&pos_y>94&&pos_y<100) pos_y=94;
    if(pos_x>160&&pos_x<166&&pos_y>94&&pos_y<176) pos_x=166;
    if(pos_x>114&&pos_x<166&&pos_y>170&&pos_y<176) pos_y=176;
    if(pos_x>114&&pos_x<166&&pos_y>184&&pos_y<190) pos_y=184;
    if(pos_x>114&&pos_x<120&&pos_y>184&&pos_y<430) pos_x=114;
    if(pos_x>160&&pos_x<166&&pos_y>184&&pos_y<430) pos_x=166;
    if(pos_x>174&&pos_x<180&&pos_y>34&&pos_y<236) pos_x=174;
    if(pos_x>220&&pos_x<226&&pos_y>34&&pos_y<236) pos_x=226;
    if(pos_x>174&&pos_x<286&&pos_y>230&&pos_y<236) pos_y=236;
    if(pos_x>174&&pos_x<286&&pos_y>184&&pos_y<190) pos_y=184;
    if(pos_x>280&&pos_x<286&&pos_y>154&&pos_y<236) pos_x=286;
    if(pos_x>234&&pos_x<240&&pos_y>154&&pos_y<236) pos_x=234;
    if(pos_x>234&&pos_x<286&&pos_y>154&&pos_y<160) pos_y=154;
    if(pos_x>174&&pos_x<180&&pos_y>244&&pos_y<416) pos_x=174;
    if(pos_x>226&&pos_x<234&&pos_y>244&&pos_y<416) pos_x=234;
    if(pos_x>174&&pos_x<286&&pos_y>244&&pos_y<250) pos_y=244;
    if(pos_x>174&&pos_x<286&&pos_y>290&&pos_y<296) pos_y=296;
    if(pos_x>174&&pos_x<226&&pos_y>410&&pos_y<416) pos_y=416;
    if(pos_x>280&&pos_x<286&&pos_y>244&&pos_y<296) pos_x=286;
    if(pos_x>234&&pos_x<240&&pos_y>304&&pos_y<416) pos_x=234;
    if(pos_x>280&&pos_x<286&&pos_y>304&&pos_y<416) pos_x=286;
    if(pos_x>234&&pos_x<286&&pos_y>304&&pos_y<310) pos_y=304;
    if(pos_x>234&&pos_x<286&&pos_y>410&&pos_y<416) pos_y=416;
    if(pos_x>234&&pos_x<346&&pos_y>34&&pos_y<40) pos_y=34;
    if(pos_x>234&&pos_x<346&&pos_y>94&&pos_y<100) pos_y=94;  //35
    if(pos_x>234&&pos_x<346&&pos_y>80&&pos_y<86) pos_y=86;
    if(pos_x>234&&pos_x<346&&pos_y>140&&pos_y<146) pos_y=146;
    if(pos_x>234&&pos_x<240&&pos_y>34&&pos_y<86) pos_x=234;
    if(pos_x>234&&pos_x<240&&pos_y>94&&pos_y<146) pos_x=234;
    if(pos_x>264&&pos_x<270&&pos_y>64&&pos_y<110) pos_x=264; //40
    if(pos_x>310&&pos_x<316&&pos_y>64&&pos_y<110) pos_x=316;
    if(pos_x>340&&pos_x<346&&pos_y>34&&pos_y<86) pos_x=346;
    if(pos_x>340&&pos_x<346&&pos_y>94&&pos_y<146) pos_x=346;
    if(pos_x>354&&pos_x<436&&pos_y>50&&pos_y<56) pos_y=56;
    if(pos_x>354&&pos_x<360&&pos_y>25&&pos_y<56) pos_x=354;  //45
    if(pos_x>430&&pos_x<436&&pos_y>25&&pos_y<56) pos_x=436;
    if(pos_x>354&&pos_x<496&&pos_y>110&&pos_y<116) pos_y=116;
    if(pos_x>354&&pos_x<360&&pos_y>64&&pos_y<116) pos_x=354;
    if(pos_x>354&&pos_x<496&&pos_y>64&&pos_y<70) pos_y=64;
    if(pos_x>444&&pos_x<450&&pos_y>34&&pos_y<110) pos_x=444;  //50
    if(pos_x>444&&pos_x<496&&pos_y>34&&pos_y<40) pos_y=34;
    if(pos_x>490&&pos_x<496&&pos_y>34&&pos_y<110) pos_x=496;
    if(pos_x>294&&pos_x<300&&pos_y>154&&pos_y<236) pos_x=294;
    if(pos_x>294&&pos_x<346&&pos_y>154&&pos_y<160) pos_y=154;
    if(pos_x>340&&pos_x<346&&pos_y>154&&pos_y<236) pos_x=346;  //55
    if(pos_x>294&&pos_x<376&&pos_y>230&&pos_y<236) pos_y=236;
    if(pos_x>294&&pos_x<376&&pos_y>184&&pos_y<190) pos_y=184;
    if(pos_x>370&&pos_x<376&&pos_y>184&&pos_y<236) pos_x=376;
    if(pos_x>294&&pos_x<300&&pos_y>244&&pos_y<430) pos_x=294;
    if(pos_x>340&&pos_x<346&&pos_y>244&&pos_y<430) pos_x=346;  //60
    if(pos_x>294&&pos_x<436&&pos_y>244&&pos_y<250) pos_y=244;
    if(pos_x>294&&pos_x<436&&pos_y>290&&pos_y<296) pos_y=296;
    if(pos_x>384&&pos_x<390&&pos_y>124&&pos_y<296) pos_x=384;
    if(pos_x>430&&pos_x<436&&pos_y>154&&pos_y<296) pos_x=436;
    if(pos_x>354&&pos_x<436&&pos_y>124&&pos_y<130) pos_y=124;  //65
    if(pos_x>354&&pos_x<436&&pos_y>170&&pos_y<176) pos_y=176;
    if(pos_x>354&&pos_x<360&&pos_y>124&&pos_y<176) pos_x=354;
    if(pos_x>354&&pos_x<436&&pos_y>304&&pos_y<310) pos_y=304;
    if(pos_x>354&&pos_x<436&&pos_y>350&&pos_y<356) pos_y=356;
    if(pos_x>354&&pos_x<360&&pos_y>304&&pos_y<356) pos_x=354;  //70
    if(pos_x>430&&pos_x<436&&pos_y>304&&pos_y<356) pos_x=436;
    if(pos_x>354&&pos_x<496&&pos_y>410&&pos_y<416) pos_y=416;
    if(pos_x>354&&pos_x<360&&pos_y>364&&pos_y<416) pos_x=354;
    if(pos_x>354&&pos_x<496&&pos_y>364&&pos_y<370) pos_y=364;
    if(pos_x>444&&pos_x<450&&pos_y>154&&pos_y<386) pos_x=444;  //75
    if(pos_x>490&&pos_x<496&&pos_y>154&&pos_y<416) pos_x=496;
    if(pos_x>444&&pos_x<570&&pos_y>154&&pos_y<160) pos_y=154;
    if(pos_x>444&&pos_x<550&&pos_y>200&&pos_y<206) pos_y=206;
    if(pos_x>530&&pos_x<570&&pos_y>214&&pos_y<220) pos_y=214;
    
    
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSfond_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

if(tour_led_map1<125)
    {SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSled_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}


if(direction==1){
    	if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
    lpDDSxwing=DDLoadBitmap(lpDD,"data/xwing_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
         
if(direction==2){
if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
    lpDDSxwing=DDLoadBitmap(lpDD,"data/xwing_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

if(direction==3){
if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
    lpDDSxwing=DDLoadBitmap(lpDD,"data/xwing_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
         
if(direction==4){
if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		} 
    lpDDSxwing=DDLoadBitmap(lpDD,"data/xwing_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}  
         
                       
    if(direction_tie1==1){
   if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
	
    if(direction_tie1==2){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie1==3){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie1==4){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}



    if(direction_tie2==1){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
	
    if(direction_tie2==2){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie2==3){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie2==4){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/tie_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}


    if(direction_tie3==1){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/g_tie_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
	
    if(direction_tie3==2){
        if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/g_tie_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie3==3){
       if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/g_tie_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie3==4){
       if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/g_tie_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

if(lpDDScalque_sortie_map1 != NULL)
		{
			lpDDScalque_sortie_map1->Release();
			lpDDScalque_sortie_map1=NULL;
		}  
	lpDDScalque_sortie_map1=DDLoadBitmap(lpDD,"data/calque_sortie_map1.bmp");

SetRect(&rc,0,0,50,50);
         if (FAILED (lpDDSBack->BltFast (533 , 200, lpDDScalque_sortie_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;
if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
{


if(lpDDSmenu_de_jeu != NULL)
		{
			lpDDSmenu_de_jeu->Release();
			lpDDSmenu_de_jeu=NULL;
		}
        
        lpDDSmenu_de_jeu=DDLoadBitmap(lpDD,"data/menu_de_jeu.bmp");
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSmenu_de_jeu, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

	if(menu_jeu==1){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 179, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }
    if(menu_jeu==2){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 219, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==3){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 249, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==4){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 289, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	                        	
}

	
else{	

deplacement_tie1();
deplacement_tie2();
deplacement_tie3();


if(niveau>=2){
deplacement_tie1();
deplacement_tie2();
deplacement_tie3();
}

if(niveau>=3){
deplacement_tie1();
deplacement_tie2();

}

if(pos_x>535) phase_de_jeu=5;
    tour++;
    tour_led_map1++;
    if(tour_led_map1>=150) tour_led_map1=1;
}



if(pos_x-x_tie1>-15&&pos_x-x_tie1<15&&pos_y-y_tie1>-15&&pos_y-y_tie1<15)
phase_de_jeu=-4;

if(pos_x-x_tie2>-15&&pos_x-x_tie2<15&&pos_y-y_tie2>-15&&pos_y-y_tie2<15)
phase_de_jeu=-4;

if(pos_x-x_tie3>-15&&pos_x-x_tie3<15&&pos_y-y_tie3>-15&&pos_y-y_tie3<15)
phase_de_jeu=-4;



    
lpDDSPrim->Flip(NULL, DDFLIP_WAIT);

return 0;}





int phase_4neg(void)
{    if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}   
	lpDDStexte=DDLoadBitmap(lpDD,"data/fond_texte.bmp");
    DDText(lpDDStexte, "La Rebellion a perdue tout contact avec vous... Vous avez échoué...", 10, 30, RGB(255,255,0));


    RECT rc;
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDStexte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
	

	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);
return 0;}



int phase_5(void)
{
 
if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}  
lpDDStexte=DDLoadBitmap(lpDD,"data/fond_texte.bmp");
    DDText(lpDDStexte, "Sans que l'ennemi ne vous détecte, vous vous êtes posé dans l'Etoile Noire.", 10, 30, RGB(255,255,0));
    DDText(lpDDStexte, "Mais maintenant, il va vous falloir être le plus discret possible, car votre vaisseau", 10, 60, RGB(255,255,0));
    DDText(lpDDStexte, "aura surement été repéré. N'oubliez pas que l'usage de vos armes ne ferait qu'attirer", 10, 90, RGB(255,255,0));
    DDText(lpDDStexte, "sur vous des ennemis encore plus nombreux.", 10, 120, RGB(255,255,0));
    DDText(lpDDStexte, "Il vous faut donc traverser le hangar dans lequel vous avez atteri, puis franchir les", 10, 150, RGB(255,255,0));
	DDText(lpDDStexte, "postes de commande afin d'arriver normalement là où l'Empire a caché nos plans", 10, 180, RGB(255,255,0));
	DDText(lpDDStexte, "secrets.", 10, 210, RGB(255,255,0));
    DDText(lpDDStexte, "Que la force soit avec vous !", 300, 370, RGB(255,255,0));
   

     x_imp1=410;
     y_imp1=250;
     direction_imp1=1;
     tour_position_imp1=1;
     tour_imp1=500;
     
     x_imp3=590;
     y_imp3=160;
     direction_imp3=4;
     tour_imp3=999;
     
     x_imp2=590;
     y_imp2=160;
     direction_imp2=4;
     tour_imp2=999;
     
     pos_x=130;
     pos_y=150;
     direction=2;
     
     tour_cinematique=1;
     
     
            RECT rc;
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDStexte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

            


	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);
		
return 0;}


int phase_6(void)
{



correction_hasard();




if(tour_imp1==998||tour_imp1==999||tour_imp1==1000)
{
     x_imp3=560;
     y_imp3=210;
     direction_imp3=4;
     tour_imp3=1000;
     
     x_imp2=560;
     y_imp2=210;
     direction_imp2=4;
     tour_imp2=1000;
     
     tour_cinematique=2;
}


    if(pos_x<50)    pos_x=50;
    if(pos_x>560&&pos_y<124)   pos_x=560;
    if(pos_x>560&&pos_y>155)   pos_x=560;
    if(pos_y<4)    pos_y=4;
    if(pos_y>420)   pos_y=420;
    if(pos_x>54&&pos_x<60&&pos_y>44&&pos_y<206) pos_x=54;
    if(pos_x>100&&pos_x<114&&pos_y>44&&pos_y<206) pos_x=114;
    if(pos_x>54&&pos_x<230&&pos_y>94&&pos_y<100) pos_y=94;
    if(pos_x>54&&pos_x<230&&pos_y>140&&pos_y<146) pos_y=146;
    if(pos_x>220&&pos_x<230&&pos_y>96&&pos_y<146) pos_x=230;  //5   
    if(pos_x>54&&pos_x<196&&pos_y>44&&pos_y<50) pos_y=44;
    if(pos_x>54&&pos_x<196&&pos_y>80&&pos_y<86) pos_y=86;
    if(pos_x>54&&pos_x<196&&pos_y>164&&pos_y<170) pos_y=164;
    if(pos_x>54&&pos_x<196&&pos_y>200&&pos_y<206) pos_y=206;
    if(pos_x>190&&pos_x<196&&pos_y>44&&pos_y<86) pos_x=196;  //10 
    if(pos_x>190&&pos_x<196&&pos_y>164&&pos_y<206) pos_x=196;
    if(pos_x>54&&pos_x<230&&pos_y>264&&pos_y<270) pos_y=264;
    if(pos_x>54&&pos_x<230&&pos_y>364&&pos_y<370) pos_y=364;
    if(pos_x>54&&pos_x<230&&pos_y>320&&pos_y<326) pos_y=326;
    if(pos_x>54&&pos_x<230&&pos_y>410&&pos_y<420) pos_y=420;  //15   
    if(pos_x>54&&pos_x<60&&pos_y>264&&pos_y<326) pos_x=54;
    if(pos_x>54&&pos_x<60&&pos_y>364&&pos_y<420) pos_x=54;
    if(pos_x>220&&pos_x<230&&pos_y>264&&pos_y<326) pos_x=230;
    if(pos_x>220&&pos_x<230&&pos_y>364&&pos_y<420) pos_x=230;
    if(pos_x>234&&pos_x<240&&pos_y>0&&pos_y<117) pos_x=234;  //20
    if(pos_x>234&&pos_x<240&&pos_y>154&&pos_y<440) pos_x=234;
    if(pos_x>280&&pos_x<287&&pos_y>0&&pos_y<117) pos_x=287;
    if(pos_x>280&&pos_x<287&&pos_y>154&&pos_y<440) pos_x=287;
    if(pos_x>234&&pos_x<287&&pos_y>110&&pos_y<124) pos_y=124;
    if(pos_x>234&&pos_x<287&&pos_y>154&&pos_y<160) pos_y=154;  //25
    if(pos_x>294&&pos_x<406&&pos_y>30&&pos_y<40) pos_y=30;
    if(pos_x>414&&pos_x<556&&pos_y>30&&pos_y<40) pos_y=30;
    if(pos_x>294&&pos_x<300&&pos_y>34&&pos_y<184) pos_x=294;
    if(pos_x>550&&pos_x<556&&pos_y>34&&pos_y<184) pos_x=556;
    if(pos_x>400&&pos_x<406&&pos_y>34&&pos_y<66) pos_x=406;  //30
    if(pos_x>414&&pos_x<420&&pos_y>34&&pos_y<66) pos_x=414;
    if(pos_x>294&&pos_x<346&&pos_y>180&&pos_y<186) pos_y=186;
    if(pos_x>504&&pos_x<556&&pos_y>180&&pos_y<186) pos_y=186;
    if(pos_x>340&&pos_x<346&&pos_y>64&&pos_y<184) pos_x=346;
    if(pos_x>504&&pos_x<510&&pos_y>64&&pos_y<184) pos_x=504;  //35
    if(pos_x>330&&pos_x<406&&pos_y>60&&pos_y<66) pos_y=66;
    if(pos_x>414&&pos_x<530&&pos_y>60&&pos_y<66) pos_y=66;
    if(pos_x>354&&pos_x<496&&pos_y>89&&pos_y<95) pos_y=89;
    if(pos_x>354&&pos_x<496&&pos_y>110&&pos_y<120) pos_y=120;
    if(pos_x>354&&pos_x<360&&pos_y>94&&pos_y<120) pos_x=354;  //40
    if(pos_x>490&&pos_x<496&&pos_y>94&&pos_y<120) pos_x=496;
    if(pos_x>354&&pos_x<496&&pos_y>150&&pos_y<160) pos_y=150;
    if(pos_x>354&&pos_x<496&&pos_y>230&&pos_y<240) pos_y=240;
    if(pos_x>354&&pos_x<360&&pos_y>150&&pos_y<236) pos_x=354;
    if(pos_x>490&&pos_x<496&&pos_y>150&&pos_y<236) pos_x=496; //45
    if(pos_x>294&&pos_x<556&&pos_y>340&&pos_y<350) pos_y=340;
    if(pos_x>294&&pos_x<556&&pos_y>360&&pos_y<370) pos_y=370;
    if(pos_x>504&&pos_x<510&&pos_y>360&&pos_y<440) pos_x=504;
    if(pos_x>550&&pos_x<556&&pos_y>350&&pos_y<440) pos_x=556;
    if(pos_x>294&&pos_x<300&&pos_y>350&&pos_y<370) pos_x=294; //50              
    if(pos_x>294&&pos_x<300&&pos_y>214&&pos_y<310) pos_x=294;
    if(pos_x>340&&pos_x<346&&pos_y>214&&pos_y<310) pos_x=346;
    if(pos_x>294&&pos_x<406&&pos_y>280&&pos_y<290) pos_y=280;
    if(pos_x>294&&pos_x<406&&pos_y>310&&pos_y<316) pos_y=316;
    if(pos_x>294&&pos_x<354&&pos_y>210&&pos_y<220) pos_y=210; //55    
    if(pos_x>400&&pos_x<410&&pos_y>280&&pos_y<316) pos_x=410;
    if(pos_x>510&&pos_x<556&&pos_y>210&&pos_y<220) pos_y=210;
    if(pos_x>414&&pos_x<556&&pos_y>280&&pos_y<290) pos_y=280;
    if(pos_x>414&&pos_x<556&&pos_y>306&&pos_y<314) pos_y=314;
    if(pos_x>414&&pos_x<420&&pos_y>284&&pos_y<314) pos_x=414;  //60
    if(pos_x>504&&pos_x<510&&pos_y>214&&pos_y<305) pos_x=504;
    if(pos_x>550&&pos_x<556&&pos_y>214&&pos_y<310) pos_x=556;
    
    if(tour_cinematique==1)
    {if(pos_x>230) pos_x=230;}
    
    if(tour_cinematique==2)
    {if(pos_x>560) pos_x=560;
    if(pos_x>495&&pos_x<520&&pos_y>375&&pos_y<380) tour_cinematique=3;}
    
    if(pos_x>600)phase_de_jeu=7;
    
    
                                              
                RECT rc;
            
	SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSfond_map2, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;



if(tour_imp1<1000){
if(lpDDScalque_sortie_map1 != NULL)
		{
			lpDDScalque_sortie_map1->Release();
			lpDDScalque_sortie_map1=NULL;
		}  
	lpDDScalque_sortie_map1=DDLoadBitmap(lpDD,"data/calque_ecran_dv.bmp");

SetRect(&rc,0,0,109,35);
         if (FAILED (lpDDSBack->BltFast (386 , 233, lpDDScalque_sortie_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;
}


if(compteur_led<=20){	
SetRect(&rc,0,0,170,20);
         if (FAILED (lpDDSBack->BltFast (325 , 372, lpDDSled_map2, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

if(lpDDSluke != NULL)
		{
			lpDDSluke->Release();
			lpDDSluke=NULL;
		}

     if(direction==1){
    if(tour_position==1){    lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h1.bmp");}
    if(tour_position==2){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h2.bmp");}
    if(tour_position==3){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h3.bmp");}
    if(tour_position==4){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h4.bmp");}
    if(tour_position==5){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h5.bmp");}
    if(tour_position==6){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h6.bmp");}
  }
         
   	
    if(direction==2){
    if(tour_position==1){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d1.bmp");}
    if(tour_position==2){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d2.bmp");}
    if(tour_position==3){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d3.bmp");}
    if(tour_position==4){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d4.bmp");}
    if(tour_position==5){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d5.bmp");}
    if(tour_position==6){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d6.bmp");}
       }
         
    if(direction==3){
    if(tour_position==1){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b1.bmp");}
    if(tour_position==2){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b2.bmp");}
    if(tour_position==3){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b3.bmp");}
    if(tour_position==4){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b4.bmp");}
    if(tour_position==5){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b5.bmp");}
    if(tour_position==6){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b6.bmp");}
 }
         
    if(direction==4){
    if(tour_position==1){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g1.bmp");}
    if(tour_position==2){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g2.bmp");}
    if(tour_position==3){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g3.bmp");}
    if(tour_position==4){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g4.bmp");}
    if(tour_position==5){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g5.bmp");}
    if(tour_position==6){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g6.bmp");}
    }


        
         
           SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSluke, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;         

 
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

 

    if(direction_imp1==1){
    if(tour_position_imp1>=1&&tour_position_imp1<4){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h1.bmp");}
    if(tour_position_imp1>=4&&tour_position_imp1<8){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h2.bmp");}
    if(tour_position_imp1>=8&&tour_position_imp1<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h3.bmp");}
    if(tour_position_imp1>=12&&tour_position_imp1<16){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h4.bmp");}
    if(tour_position_imp1>=16&&tour_position_imp1<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h5.bmp");}
    if(tour_position_imp1>=20&&tour_position_imp1<24){lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h6.bmp");}
    }
    


	
    if(direction_imp1==2){
       if(tour_position_imp1>=1&&tour_position_imp1<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d1.bmp");}
    if(tour_position_imp1>=4&&tour_position_imp1<8){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d2.bmp");}
    if(tour_position_imp1>=8&&tour_position_imp1<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d3.bmp");}
    if(tour_position_imp1>=12&&tour_position_imp1<16){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d4.bmp");}
    if(tour_position_imp1>=16&&tour_position_imp1<20){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d5.bmp");}
    if(tour_position_imp1>=20&&tour_position_imp1<24){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d6.bmp");}
     }



    if(direction_imp1==3){
    if(tour_position_imp1>=1&&tour_position_imp1<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b1.bmp");}
    if(tour_position_imp1>=4&&tour_position_imp1<8){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b2.bmp");}
    if(tour_position_imp1>=8&&tour_position_imp1<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b3.bmp");}
    if(tour_position_imp1>=12&&tour_position_imp1<16){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b4.bmp");}
    if(tour_position_imp1>=16&&tour_position_imp1<20){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b5.bmp");}
    if(tour_position_imp1>=20&&tour_position_imp1<24){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b6.bmp");}
     }



    if(direction_imp1==4){
    if(tour_position_imp1>=1&&tour_position_imp1<4){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g1.bmp");}
    if(tour_position_imp1>=4&&tour_position_imp1<8){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g2.bmp");}
    if(tour_position_imp1>=8&&tour_position_imp1<12){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g3.bmp");}
    if(tour_position_imp1>=12&&tour_position_imp1<16){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g4.bmp");}
    if(tour_position_imp1>=16&&tour_position_imp1<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g5.bmp");}
    if(tour_position_imp1>=20&&tour_position_imp1<24){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g6.bmp");}}


   SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_imp1, y_imp1, lpDDSimp, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;      
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/

 

   if(direction_imp2==1){
    if(tour_position_imp2>=1&&tour_position_imp2<4){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h1.bmp");}
    if(tour_position_imp2>=4&&tour_position_imp2<8){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h2.bmp");}
    if(tour_position_imp2>=2&&tour_position_imp2<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h3.bmp");}
    if(tour_position_imp2>=12&&tour_position_imp2<16){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h4.bmp");}
    if(tour_position_imp2>=16&&tour_position_imp2<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h5.bmp");}
    if(tour_position_imp2>=20&&tour_position_imp2<24){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_h6.bmp");}
    }
    


	
    if(direction_imp2==2){
       if(tour_position_imp2>=1&&tour_position_imp2<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d1.bmp");}
    if(tour_position_imp2>=4&&tour_position_imp2<8){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d2.bmp");}
    if(tour_position_imp2>=8&&tour_position_imp2<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d3.bmp");}
    if(tour_position_imp2>=12&&tour_position_imp2<16){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d4.bmp");}
    if(tour_position_imp2>=16&&tour_position_imp2<20){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d5.bmp");}
    if(tour_position_imp2>=20&&tour_position_imp2<24){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_d6.bmp");}
     }



    if(direction_imp2==3){
    if(tour_position_imp2>=1&&tour_position_imp2<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b1.bmp");}
    if(tour_position_imp2>=4&&tour_position_imp2<8){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b2.bmp");}
    if(tour_position_imp2>=8&&tour_position_imp2<12){lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b3.bmp");}
    if(tour_position_imp2>=12&&tour_position_imp2<16){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b4.bmp");}
    if(tour_position_imp2>=16&&tour_position_imp2<20){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b5.bmp");}
    if(tour_position_imp2>=20&&tour_position_imp2<24){lpDDSimp=DDLoadBitmap(lpDD,"data/imp_b6.bmp");}
     }



    if(direction_imp2==4){
    if(tour_position_imp2>=1&&tour_position_imp2<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g1.bmp");}
    if(tour_position_imp2>=4&&tour_position_imp2<8){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g2.bmp");}
    if(tour_position_imp2>=8&&tour_position_imp2<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g3.bmp");}
    if(tour_position_imp2>=12&&tour_position_imp2<16){  lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g4.bmp");}
    if(tour_position_imp2>=16&&tour_position_imp2<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g5.bmp");}
    if(tour_position_imp2>=20&&tour_position_imp2<24){   lpDDSimp=DDLoadBitmap(lpDD,"data/imp_g6.bmp");}}
      
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/
       
          SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_imp2, y_imp2, lpDDSimp, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;     
      	



if(direction_imp3==1){
    if(tour_position_imp3>=1&&tour_position_imp3<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_h1.bmp");}
    if(tour_position_imp3>=4&&tour_position_imp3<8){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_h2.bmp");}
    if(tour_position_imp3>=8&&tour_position_imp3<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_h3.bmp");}
    if(tour_position_imp3>=12&&tour_position_imp3<16){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_h4.bmp");}
    if(tour_position_imp3>=16&&tour_position_imp3<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_h5.bmp");}
    if(tour_position_imp3>=20&&tour_position_imp3<24){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_h6.bmp");}
    }
    


	
    if(direction_imp3==2){
       if(tour_position_imp3>=1&&tour_position_imp3<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_d1.bmp");}
    if(tour_position_imp3>=4&&tour_position_imp3<8){   lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_d2.bmp");}
    if(tour_position_imp3>=8&&tour_position_imp3<12){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_d3.bmp");}
    if(tour_position_imp3>=12&&tour_position_imp3<16){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_d4.bmp");}
    if(tour_position_imp3>=16&&tour_position_imp3<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_d5.bmp");}
    if(tour_position_imp3>=20&&tour_position_imp3<24){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_d6.bmp");}
     }



    if(direction_imp3==3){
    if(tour_position_imp3>=1&&tour_position_imp3<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_b1.bmp");}
    if(tour_position_imp3>=4&&tour_position_imp3<8){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_b2.bmp");}
    if(tour_position_imp3>=8&&tour_position_imp3<12){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_b3.bmp");}
    if(tour_position_imp3>=12&&tour_position_imp3<16){   lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_b4.bmp");}
    if(tour_position_imp3>=16&&tour_position_imp3<20){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_b5.bmp");}
    if(tour_position_imp3>=20&&tour_position_imp3<24){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_b6.bmp");}
     }



    if(direction_imp3==4){
    if(tour_position_imp3>=1&&tour_position_imp3<4){  lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_g1.bmp");}
    if(tour_position_imp3>=4&&tour_position_imp3<8){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_g2.bmp");}
    if(tour_position_imp3>=8&&tour_position_imp3<12){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_g3.bmp");}
    if(tour_position_imp3>=12&&tour_position_imp3<16){   lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_g4.bmp");}
    if(tour_position_imp3>=16&&tour_position_imp3<20){   lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_g5.bmp");}
    if(tour_position_imp3>=20&&tour_position_imp3<24){ lpDDSimp=DDLoadBitmap(lpDD,"data/g_imp_g6.bmp");}}


   SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_imp3, y_imp3, lpDDSimp, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;      
/*************************************************************************/
/*************************************************************************/
/*************************************************************************/


if(tour_cinematique==1)
{if(lpDDSporte != NULL)
		{
			lpDDSporte->Release();
			lpDDSporte=NULL;
		} 
lpDDSporte=DDLoadBitmap(lpDD,"data/porte.bmp");

SetRect(&rc,0,0,31,53);
         if (FAILED (lpDDSBack->BltFast (260, 121, lpDDSporte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;      

}

if(tour_cinematique==2)
{if(lpDDSporte != NULL)
		{
			lpDDSporte->Release();
			lpDDSporte=NULL;
		} 
lpDDSporte=DDLoadBitmap(lpDD,"data/porte.bmp");

SetRect(&rc,0,0,31,53);
         if (FAILED (lpDDSBack->BltFast (590, 121, lpDDSporte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;      

}

if(lpDDScalque_vaisseau_map2 != NULL)
		{
			lpDDScalque_vaisseau_map2->Release();
			lpDDScalque_vaisseau_map2=NULL;
		} 
        
 if(tour_imp1<750){
  lpDDScalque_vaisseau_map2=DDLoadBitmap(lpDD,"data/calque_vaisseau_map2.bmp");
  DDText(lpDDScalque_vaisseau_map2, "Seigneur Vador,", 360, 160, RGB(255,255,0)); 
  DDText(lpDDScalque_vaisseau_map2, "Un vaisseau Rebelle de type X-Wing", 360, 185, RGB(255,255,0));
  DDText(lpDDScalque_vaisseau_map2, "vient de se poser dans le hangar.", 360, 210, RGB(255,255,0));  
}
 

if(tour_imp1>=750&&tour_imp1<1000){
  lpDDScalque_vaisseau_map2=DDLoadBitmap(lpDD,"data/calque_vaisseau_map2.bmp");
  DDText(lpDDScalque_vaisseau_map2, "C'est ce que je craignais.", 360, 160, RGB(255,0,0)); 
  DDText(lpDDScalque_vaisseau_map2, "Je vous envoie des hommes pour arrêter", 360, 185, RGB(255,0,0));
  DDText(lpDDScalque_vaisseau_map2, "le pilote, mais je le veux vivant.", 360, 210, RGB(255,0,0));  
}
                
 if(tour_imp1>=1000){
  lpDDScalque_vaisseau_map2=DDLoadBitmap(lpDD,"data/calque_vaisseau_map2.bmp");
   
}	
    
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDScalque_vaisseau_map2, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
		
if(tour_imp1<1000)tour_imp1++;



if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
{


if(lpDDSmenu_de_jeu != NULL)
		{
			lpDDSmenu_de_jeu->Release();
			lpDDSmenu_de_jeu=NULL;
		}
        
        lpDDSmenu_de_jeu=DDLoadBitmap(lpDD,"data/menu_de_jeu.bmp");
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSmenu_de_jeu, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

	if(menu_jeu==1){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 179, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }
    if(menu_jeu==2){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 219, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==3){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 249, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==4){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 289, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	                        	
}

	
else{
	
deplacement_imp1();
if(tour_imp1>1000){
deplacement_imp1();
deplacement_imp2();	
deplacement_imp3();
deplacement_imp2();	
deplacement_imp3();}

if(niveau>=2){
deplacement_imp1();
if(tour_imp1>1000){
deplacement_imp2();	
deplacement_imp3();}
}

if(niveau>=3){
deplacement_imp1();
if(tour_imp1>1000){
deplacement_imp2();	
deplacement_imp3();}
}

}

if(pos_x-x_imp1>-30&&pos_x-x_imp1<30&&pos_y-y_imp1>-30&&pos_y-y_imp1<30)
phase_de_jeu=-4;
if(pos_x-x_imp2>-30&&pos_x-x_imp2<30&&pos_y-y_imp2>-30&&pos_y-y_imp2<30)
phase_de_jeu=-4;
if(pos_x-x_imp3>-30&&pos_x-x_imp3<30&&pos_y-y_imp3>-30&&pos_y-y_imp3<30)
phase_de_jeu=-4;

compteur_led++;	
if(compteur_led==25)compteur_led=1;


lpDDSPrim->Flip(NULL, DDFLIP_WAIT);

return 0;}
//***************************************************************************
//***************************************************************************
//***************************************************************************



int phase_7(void)
{

     x_dv=330;
     y_dv=210;
     direction_dv=4;
     tour_position_dv=1;

force=30;
     
     pos_x=50;
     pos_y=210;
     direction=2;
     
     tour_cinematique=1;

if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}  
lpDDStexte=DDLoadBitmap(lpDD,"data/fond_texte.bmp");
    DDText(lpDDStexte, "Félicitation !", 10, 30, RGB(255,255,0));
    DDText(lpDDStexte, "Vous avez réussi à vous infilter jusqu'à votre but : là où l'Empire", 10, 60, RGB(255,255,0));
    DDText(lpDDStexte, "tente de décoder les plans secrets du V-Wing.", 10, 90, RGB(255,255,0));
    DDText(lpDDStexte, "Vous devez à tout prix récupérer nos données ou la Rebellion ne sera bientôt", 10, 120, RGB(255,255,0));
    DDText(lpDDStexte, "plus qu'un souvenir.", 10, 150, RGB(255,255,0));
    DDText(lpDDStexte, "Que la force soit avec vous !", 300, 370, RGB(255,255,0));
   
   

    
     
     
            RECT rc;
            
	SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDStexte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
	

	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);
		
return 0;}



int phase_8(void)
{



correction_hasard();

    RECT rc;

    if(pos_x<50)    pos_x=50;
    if(pos_x>530)   pos_x=530;
    if(pos_y<30)    pos_y=30;
    if(pos_y>420)   pos_y=420;
  
    
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSfond_map3, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
		
		if(tour_cinematique==1||tour_cinematique==3)
		{
		SetRect(&rc,0,0,40,25);
	if (FAILED (lpDDSBack->BltFast (353, 217, lpDDSpasserelle, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}

if(force>=10){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 416, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=20){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 382, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=30){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 348, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=40){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 314, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=50){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 280, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=60){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 246, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=70){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 212, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=80){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 178, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=90){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 145, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}
if(force>=100){	SetRect(&rc,0,0,45,22);	if (FAILED (lpDDSBack->BltFast (578, 111, lpDDSforce, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}	
            
                    		
if(lpDDSluke != NULL)
		{
			lpDDSluke->Release();
			lpDDSluke=NULL;
		}

     if(direction==1){
    if(tour_position==1){    lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h1.bmp");}
    if(tour_position==2){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h2.bmp");}
    if(tour_position==3){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h3.bmp");}
    if(tour_position==4){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h4.bmp");}
    if(tour_position==5){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h5.bmp");}
    if(tour_position==6){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_h6.bmp");}
  }
         
   	
    if(direction==2){
    if(tour_position==1){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d1.bmp");}
    if(tour_position==2){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d2.bmp");}
    if(tour_position==3){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d3.bmp");}
    if(tour_position==4){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d4.bmp");}
    if(tour_position==5){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d5.bmp");}
    if(tour_position==6){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_d6.bmp");}
       }
         
    if(direction==3){
    if(tour_position==1){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b1.bmp");}
    if(tour_position==2){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b2.bmp");}
    if(tour_position==3){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b3.bmp");}
    if(tour_position==4){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b4.bmp");}
    if(tour_position==5){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b5.bmp");}
    if(tour_position==6){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_b6.bmp");}
 }
         
    if(direction==4){
    if(tour_position==1){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g1.bmp");}
    if(tour_position==2){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g2.bmp");}
    if(tour_position==3){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g3.bmp");}
    if(tour_position==4){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g4.bmp");}
    if(tour_position==5){ lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g5.bmp");}
    if(tour_position==6){  lpDDSluke=DDLoadBitmap(lpDD,"data/luke_g6.bmp");}
    }


        
         
           SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSluke, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;         

if(tour_cinematique==1||tour_cinematique==2){
if(lpDDSdv != NULL)
		{
			lpDDSdv->Release();
			lpDDSdv=NULL;
		}

     if(direction_dv==1){
    if(tour_position_dv>=1&&tour_position_dv<4){    lpDDSdv=DDLoadBitmap(lpDD,"data/dv_h1.bmp");}
    if(tour_position_dv>=4&&tour_position_dv<8){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_h2.bmp");}
    if(tour_position_dv>=8&&tour_position_dv<12){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_h3.bmp");}
    if(tour_position_dv>=12&&tour_position_dv<16){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_h4.bmp");}
    if(tour_position_dv>=16&&tour_position_dv<20){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_h5.bmp");}
    if(tour_position_dv>=20&&tour_position_dv<24){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_h6.bmp");}
  }
         
   	
    if(direction_dv==2){
    if(tour_position_dv>=1&&tour_position_dv<4){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_d1.bmp");}
    if(tour_position_dv>=4&&tour_position_dv<8){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_d2.bmp");}
    if(tour_position_dv>=8&&tour_position_dv<12){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_d3.bmp");}
    if(tour_position_dv>=12&&tour_position_dv<16){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_d4.bmp");}
    if(tour_position_dv>=16&&tour_position_dv<20){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_d5.bmp");}
    if(tour_position_dv>=20&&tour_position_dv<24){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_d6.bmp");}
       }
         
    if(direction_dv==3){
    if(tour_position_dv>=1&&tour_position_dv<4){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_b1.bmp");}
    if(tour_position_dv>=4&&tour_position_dv<8){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_b2.bmp");}
    if(tour_position_dv>=8&&tour_position_dv<12){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_b3.bmp");}
    if(tour_position_dv>=12&&tour_position_dv<16){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_b4.bmp");}
    if(tour_position_dv>=16&&tour_position_dv<20){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_b5.bmp");}
    if(tour_position_dv>=20&&tour_position_dv<24){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_b6.bmp");}
 }
         
    if(direction_dv==4){
    if(tour_position_dv>=1&&tour_position_dv<4){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_g1.bmp");}
    if(tour_position_dv>=4&&tour_position_dv<8){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_g2.bmp");}
    if(tour_position_dv>=8&&tour_position_dv<12){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_g3.bmp");}
    if(tour_position_dv>=12&&tour_position_dv<16){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_g4.bmp");}
    if(tour_position_dv>=16&&tour_position_dv<20){ lpDDSdv=DDLoadBitmap(lpDD,"data/dv_g5.bmp");}
    if(tour_position_dv>=20&&tour_position_dv<24){  lpDDSdv=DDLoadBitmap(lpDD,"data/dv_g6.bmp");}
    }


        
         
           SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_dv, y_dv, lpDDSdv, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;         
}

   SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDScalque_map3, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
		


if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
{


if(lpDDSmenu_de_jeu != NULL)
		{
			lpDDSmenu_de_jeu->Release();
			lpDDSmenu_de_jeu=NULL;
		}
        
        lpDDSmenu_de_jeu=DDLoadBitmap(lpDD,"data/menu_de_jeu.bmp");
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSmenu_de_jeu, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

	if(menu_jeu==1){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 179, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }
    if(menu_jeu==2){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 219, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==3){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 249, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==4){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 289, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	                        	
}

	
else{	

if(tour_cinematique==1)
{x_dv++; direction_dv=2;tour_position_dv++; if(x_dv>430) tour_cinematique=2;}
if(tour_cinematique==2){

if(pos_x<x_dv){x_dv--; direction_dv=4; tour_position_dv++;}
if(pos_x>x_dv){x_dv++; direction_dv=2; tour_position_dv++;}
if(pos_y<y_dv){y_dv--; direction_dv=1; tour_position_dv++;}
if(pos_y>y_dv){y_dv++; direction_dv=3; tour_position_dv++;}
if(niveau>=2){
if(pos_x<x_dv){x_dv--; direction_dv=4; tour_position_dv++;}
if(pos_x>x_dv){x_dv++; direction_dv=2; tour_position_dv++;}
if(pos_y<y_dv){y_dv--; direction_dv=1; tour_position_dv++;}
if(pos_y>y_dv){y_dv++; direction_dv=3; tour_position_dv++;}
}
if(niveau>=3){
if(pos_x<x_dv){x_dv--; direction_dv=4; tour_position_dv++;}
if(pos_x>x_dv){x_dv++; direction_dv=2; tour_position_dv++;}
if(pos_y<y_dv){y_dv--; direction_dv=1; tour_position_dv++;}
if(pos_y>y_dv){y_dv++; direction_dv=3; tour_position_dv++;}
}
}
if(tour_position_dv>=24)tour_position_dv=1;



  
      tour++;
    
    force+=0.1;
   
if (force>=100) force=100;  
}


//********Dark Vador***************
//*********************************




//*******Contourner le vide********

if(x_dv>180&&x_dv<190&&y_dv>120&&y_dv<275){x_dv--;}
if(x_dv>380&&x_dv<390&&y_dv>120&&y_dv<275){x_dv++;}

if(x_dv>180&&x_dv<390&&y_dv>120&&y_dv<130){y_dv--;}
if(x_dv>180&&x_dv<390&&y_dv>265&&y_dv<275){y_dv++;}
if(niveau>=2){
if(x_dv>180&&x_dv<190&&y_dv>120&&y_dv<275){x_dv--;}
if(x_dv>380&&x_dv<390&&y_dv>120&&y_dv<275){x_dv++;}

if(x_dv>180&&x_dv<390&&y_dv>120&&y_dv<130){y_dv--;}
if(x_dv>180&&x_dv<390&&y_dv>265&&y_dv<275){y_dv++;}
}
if(niveau>=3){
if(x_dv>180&&x_dv<190&&y_dv>120&&y_dv<275){x_dv--;}
if(x_dv>380&&x_dv<390&&y_dv>120&&y_dv<275){x_dv++;}

if(x_dv>180&&x_dv<390&&y_dv>120&&y_dv<130){y_dv--;}
if(x_dv>180&&x_dv<390&&y_dv>265&&y_dv<275){y_dv++;}
}

//*********************************
//*********************************
//*********************************


lpDDSPrim->Flip(NULL, DDFLIP_WAIT);

if(tour_cinematique==1||tour_cinematique==2){
if(pos_x>190&&pos_x<375&&pos_y>120&&pos_y<270)
phase_de_jeu=-4;}

if(pos_x-x_dv>-30&&pos_x-x_dv<30&&pos_y-y_dv>-30&&pos_y-y_dv<30)
phase_de_jeu=-4;

if(tour_cinematique==2){
if(x_dv>190&&x_dv<375&&y_dv>120&&y_dv<270){x_dv=600; y_dv=200;
tour_cinematique=3;}

if(tour_cinematique==3)
{if(pos_x>190&&pos_x<375&&pos_y>120&&pos_y<270)
phase_de_jeu=-4;
if(pos_x>260&&pos_x<400&&pos_y>185&&pos_y<215)
phase_de_jeu=8;


}

}
if(pos_x>300&&pos_x<330&&pos_y>160&&pos_y<240){
phase_de_jeu=9;
tour_cinematique=1;
}
return 0;}


int phase_9(void)
{

     
if(tour_cinematique==1){
if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}  
lpDDStexte=DDLoadBitmap(lpDD,"data/fond_texte.bmp");
    DDText(lpDDStexte, "Vous avez réussi à vaincre Vador et à récuperez nos plans !", 10, 30, RGB(255,255,0));
    DDText(lpDDStexte, "La Rebellion vous acclame comme un héros à votre retour à la base.", 10, 60, RGB(255,255,0));
    DDText(lpDDStexte, "Désormais, nous allons pouvoir fabriquer des V-Wing qui, espéront-le,", 10, 90, RGB(255,255,0));
    DDText(lpDDStexte, "réussiront à détruire à tout jamais la tyrannie que l'Empire fait régner sur la galaxie.", 10, 120, RGB(255,255,0));
    DDText(lpDDStexte, "Que la force soit toujours avec vous !", 250, 370, RGB(255,255,0));
 }  

   
if(tour_cinematique==2){
if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}  
lpDDStexte=DDLoadBitmap(lpDD,"data/fond_texte.bmp");
    DDText(lpDDStexte, "S-W 2  L'Appel du Destin", 225, 30, RGB(255,255,0));
    DDText(lpDDStexte, "-Studio Amanga-", 250, 100, RGB(255,255,0));
    DDText(lpDDStexte, "Responsable du projet   : Vincent Tourraine", 50, 130, RGB(255,255,0));
    DDText(lpDDStexte, "Graphiste principal     : Plo Koon", 50, 160, RGB(255,255,0));
    DDText(lpDDStexte, "Superviseur des tests   : Stéphane Caron des Studios Tastalian", 50, 190, RGB(255,255,0));
    DDText(lpDDStexte, "Testeur additionnel     : Simon Couturier", 50, 220, RGB(255,255,0));
    DDText(lpDDStexte, "Studio Amanga, 2002, S-W 2 L'Appel du Destin, tous droits réservés.", 10, 330, RGB(255,255,0));
    DDText(lpDDStexte, "Toute exploitation du logiciel ou de son contenu sans l'accord de Studio Amanga", 10, 350, RGB(255,255,0));
    DDText(lpDDStexte, "est passible de poursuite judiciare.", 10, 370, RGB(255,255,0));
    DDText(lpDDStexte, "Tous les visuels et noms des personnages et vaisseaux sont déposés par LucasFilm Ltd.", 10, 390, RGB(255,255,0));
    DDText(lpDDStexte, "S-W 2 v1.1 - 2002 - CODEXIA", 10, 445, RGB(255,255,0));
   
 }  
    
     
     
            RECT rc;
            
	SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDStexte, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;
	

	lpDDSPrim->Flip(NULL, DDFLIP_WAIT);
		
return 0;}



int phase_3spec(void)
{



correction_hasard();

    RECT rc;

    if(pos_x<50)    pos_x=50;
    if(pos_x>500&&pos_y<175)   pos_x=500;
    if(pos_x>500&&pos_y>245)   pos_x=500;
    if(pos_y<30)    pos_y=30;
    if(pos_y>420)   pos_y=420;
    if(pos_x>50&&pos_x<60&&pos_y>34&&pos_y<266) pos_x=50;
    if(pos_x>50&&pos_x<60&&pos_y>274&&pos_y<416) pos_x=50;
	if(pos_x>54&&pos_x<106&&pos_y>270&&pos_y<275) pos_y=270;
	if(pos_x>54&&pos_x<106&&pos_y>265&&pos_y<270) pos_y=270;
	if(pos_x>54&&pos_x<106&&pos_y>414&&pos_y<420) pos_y=420;
    if(pos_x>54&&pos_x<224&&pos_y>30&&pos_y<38) pos_y=30;
    if(pos_x>104&&pos_x<110&&pos_y>274&&pos_y<416) pos_x=110;
    if(pos_x>104&&pos_x<110&&pos_y>34&&pos_y<266) pos_x=110;
    if(pos_x>54&&pos_x<224&&pos_y>84&&pos_y<90) pos_y=90;
    if(pos_x>114&&pos_x<120&&pos_y>94&&pos_y<176) pos_x=114;
    if(pos_x>114&&pos_x<166&&pos_y>94&&pos_y<100) pos_y=94;
    if(pos_x>160&&pos_x<166&&pos_y>94&&pos_y<176) pos_x=166;
    if(pos_x>114&&pos_x<166&&pos_y>170&&pos_y<176) pos_y=176;
    if(pos_x>114&&pos_x<166&&pos_y>184&&pos_y<190) pos_y=184;
    if(pos_x>114&&pos_x<120&&pos_y>184&&pos_y<430) pos_x=114;
    if(pos_x>160&&pos_x<166&&pos_y>184&&pos_y<430) pos_x=166;
    if(pos_x>174&&pos_x<180&&pos_y>34&&pos_y<236) pos_x=174;
    if(pos_x>220&&pos_x<226&&pos_y>34&&pos_y<236) pos_x=226;
    if(pos_x>174&&pos_x<286&&pos_y>230&&pos_y<236) pos_y=236;
    if(pos_x>174&&pos_x<286&&pos_y>184&&pos_y<190) pos_y=184;
    if(pos_x>280&&pos_x<286&&pos_y>154&&pos_y<236) pos_x=286;
    if(pos_x>234&&pos_x<240&&pos_y>154&&pos_y<236) pos_x=234;
    if(pos_x>234&&pos_x<286&&pos_y>154&&pos_y<160) pos_y=154;
    if(pos_x>174&&pos_x<180&&pos_y>244&&pos_y<416) pos_x=174;
    if(pos_x>226&&pos_x<234&&pos_y>244&&pos_y<416) pos_x=234;
    if(pos_x>174&&pos_x<286&&pos_y>244&&pos_y<250) pos_y=244;
    if(pos_x>174&&pos_x<286&&pos_y>290&&pos_y<296) pos_y=296;
    if(pos_x>174&&pos_x<226&&pos_y>410&&pos_y<416) pos_y=416;
    if(pos_x>280&&pos_x<286&&pos_y>244&&pos_y<296) pos_x=286;
    if(pos_x>234&&pos_x<240&&pos_y>304&&pos_y<416) pos_x=234;
    if(pos_x>280&&pos_x<286&&pos_y>304&&pos_y<416) pos_x=286;
    if(pos_x>234&&pos_x<286&&pos_y>304&&pos_y<310) pos_y=304;
    if(pos_x>234&&pos_x<286&&pos_y>410&&pos_y<416) pos_y=416;
    if(pos_x>234&&pos_x<346&&pos_y>34&&pos_y<40) pos_y=34;
    if(pos_x>234&&pos_x<346&&pos_y>94&&pos_y<100) pos_y=94;  //35
    if(pos_x>234&&pos_x<346&&pos_y>80&&pos_y<86) pos_y=86;
    if(pos_x>234&&pos_x<346&&pos_y>140&&pos_y<146) pos_y=146;
    if(pos_x>234&&pos_x<240&&pos_y>34&&pos_y<86) pos_x=234;
    if(pos_x>234&&pos_x<240&&pos_y>94&&pos_y<146) pos_x=234;
    if(pos_x>264&&pos_x<270&&pos_y>64&&pos_y<110) pos_x=264; //40
    if(pos_x>310&&pos_x<316&&pos_y>64&&pos_y<110) pos_x=316;
    if(pos_x>340&&pos_x<346&&pos_y>34&&pos_y<86) pos_x=346;
    if(pos_x>340&&pos_x<346&&pos_y>94&&pos_y<146) pos_x=346;
    if(pos_x>354&&pos_x<436&&pos_y>50&&pos_y<56) pos_y=56;
    if(pos_x>354&&pos_x<360&&pos_y>25&&pos_y<56) pos_x=354;  //45
    if(pos_x>430&&pos_x<436&&pos_y>25&&pos_y<56) pos_x=436;
    if(pos_x>354&&pos_x<496&&pos_y>110&&pos_y<116) pos_y=116;
    if(pos_x>354&&pos_x<360&&pos_y>64&&pos_y<116) pos_x=354;
    if(pos_x>354&&pos_x<496&&pos_y>64&&pos_y<70) pos_y=64;
    if(pos_x>444&&pos_x<450&&pos_y>34&&pos_y<110) pos_x=444;  //50
    if(pos_x>444&&pos_x<496&&pos_y>34&&pos_y<40) pos_y=34;
    if(pos_x>490&&pos_x<496&&pos_y>34&&pos_y<110) pos_x=496;
    if(pos_x>294&&pos_x<300&&pos_y>154&&pos_y<236) pos_x=294;
    if(pos_x>294&&pos_x<346&&pos_y>154&&pos_y<160) pos_y=154;
    if(pos_x>340&&pos_x<346&&pos_y>154&&pos_y<236) pos_x=346;  //55
    if(pos_x>294&&pos_x<376&&pos_y>230&&pos_y<236) pos_y=236;
    if(pos_x>294&&pos_x<376&&pos_y>184&&pos_y<190) pos_y=184;
    if(pos_x>370&&pos_x<376&&pos_y>184&&pos_y<236) pos_x=376;
    if(pos_x>294&&pos_x<300&&pos_y>244&&pos_y<430) pos_x=294;
    if(pos_x>340&&pos_x<346&&pos_y>244&&pos_y<430) pos_x=346;  //60
    if(pos_x>294&&pos_x<436&&pos_y>244&&pos_y<250) pos_y=244;
    if(pos_x>294&&pos_x<436&&pos_y>290&&pos_y<296) pos_y=296;
    if(pos_x>384&&pos_x<390&&pos_y>124&&pos_y<296) pos_x=384;
    if(pos_x>430&&pos_x<436&&pos_y>154&&pos_y<296) pos_x=436;
    if(pos_x>354&&pos_x<436&&pos_y>124&&pos_y<130) pos_y=124;  //65
    if(pos_x>354&&pos_x<436&&pos_y>170&&pos_y<176) pos_y=176;
    if(pos_x>354&&pos_x<360&&pos_y>124&&pos_y<176) pos_x=354;
    if(pos_x>354&&pos_x<436&&pos_y>304&&pos_y<310) pos_y=304;
    if(pos_x>354&&pos_x<436&&pos_y>350&&pos_y<356) pos_y=356;
    if(pos_x>354&&pos_x<360&&pos_y>304&&pos_y<356) pos_x=354;  //70
    if(pos_x>430&&pos_x<436&&pos_y>304&&pos_y<356) pos_x=436;
    if(pos_x>354&&pos_x<496&&pos_y>410&&pos_y<416) pos_y=416;
    if(pos_x>354&&pos_x<360&&pos_y>364&&pos_y<416) pos_x=354;
    if(pos_x>354&&pos_x<496&&pos_y>364&&pos_y<370) pos_y=364;
    if(pos_x>444&&pos_x<450&&pos_y>154&&pos_y<386) pos_x=444;  //75
    if(pos_x>490&&pos_x<496&&pos_y>154&&pos_y<416) pos_x=496;
    if(pos_x>444&&pos_x<570&&pos_y>154&&pos_y<160) pos_y=154;
    if(pos_x>444&&pos_x<550&&pos_y>200&&pos_y<206) pos_y=206;
    if(pos_x>530&&pos_x<570&&pos_y>214&&pos_y<220) pos_y=214;
    
    
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSfond_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

if(tour_led_map1<125)
    {SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSled_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;}


if(direction==1){
    	if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
    lpDDSxwing=DDLoadBitmap(lpDD,"data/tie_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
         
if(direction==2){
if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
    lpDDSxwing=DDLoadBitmap(lpDD,"data/tie_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

if(direction==3){
if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
    lpDDSxwing=DDLoadBitmap(lpDD,"data/tie_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
         
if(direction==4){
if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		} 
    lpDDSxwing=DDLoadBitmap(lpDD,"data/tie_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (pos_x, pos_y, lpDDSxwing, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}  
         
                       
    if(direction_tie1==1){
   if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
	
    if(direction_tie1==2){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie1==3){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie1==4){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie1, y_tie1, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}



    if(direction_tie2==1){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
	
    if(direction_tie2==2){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie2==3){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie2==4){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		} 
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie2, y_tie2, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}


    if(direction_tie3==1){
    if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_h.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}
	
    if(direction_tie3==2){
        if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_d.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie3==3){
       if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_b.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

    if(direction_tie3==4){
       if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}  
        lpDDStie=DDLoadBitmap(lpDD,"data/xwing_g.bmp");
         SetRect(&rc,0,0,30,30);
         if (FAILED (lpDDSBack->BltFast (x_tie3, y_tie3, lpDDStie, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;}

if(lpDDScalque_sortie_map1 != NULL)
		{
			lpDDScalque_sortie_map1->Release();
			lpDDScalque_sortie_map1=NULL;
		}  
	lpDDScalque_sortie_map1=DDLoadBitmap(lpDD,"data/calque_sortie_map1.bmp");

SetRect(&rc,0,0,50,50);
         if (FAILED (lpDDSBack->BltFast (533 , 200, lpDDScalque_sortie_map1, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
         return FALSE;
if(menu_jeu==1||menu_jeu==2||menu_jeu==3||menu_jeu==4)
{


if(lpDDSmenu_de_jeu != NULL)
		{
			lpDDSmenu_de_jeu->Release();
			lpDDSmenu_de_jeu=NULL;
		}
        
        lpDDSmenu_de_jeu=DDLoadBitmap(lpDD,"data/menu_de_jeu.bmp");
	
    SetRect(&rc,0,0,640,480);
	if (FAILED (lpDDSBack->BltFast (0, 0, lpDDSmenu_de_jeu, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
		return FALSE;

	if(menu_jeu==1){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 179, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }
    if(menu_jeu==2){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 219, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==3){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (268, 249, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	
    if(menu_jeu==4){
                SetRect(&rc,0,0,17,15);
                if (FAILED (lpDDSBack->BltFast (258, 289, lpDDSselect, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY )))
                return FALSE;
                }	                        	
}

	
else{	

deplacement_tie1();
deplacement_tie1();
deplacement_tie2();
deplacement_tie2();
deplacement_tie3();
deplacement_tie3();
deplacement_tie3();

if(pos_x>535) phase_de_jeu=5;
    tour++;
    tour_led_map1++;
    if(tour_led_map1>=150) tour_led_map1=1;
}



if(pos_x-x_tie1>-15&&pos_x-x_tie1<15&&pos_y-y_tie1>-15&&pos_y-y_tie1<15)
phase_de_jeu=-4;

if(pos_x-x_tie2>-15&&pos_x-x_tie2<15&&pos_y-y_tie2>-15&&pos_y-y_tie2<15)
phase_de_jeu=-4;

if(pos_x-x_tie3>-15&&pos_x-x_tie3<15&&pos_y-y_tie3>-15&&pos_y-y_tie3<15)
phase_de_jeu=-4;



    
lpDDSPrim->Flip(NULL, DDFLIP_WAIT);

return 0;}





int deplacement_tie1(void)
{


    //temps d'attente
    
    if(tour_tie1<=0)
    {}
    
    //*************************
    //*****routine de base*****
    //*************************
    
    
    if(tour_tie1>=0&&tour_tie1<120)
    {x_tie1--;
    direction_tie1=4;}
      
    
    if(tour_tie1>=120&&tour_tie1<300)
    {y_tie1--;
    direction_tie1=1;}
    
    
    
    
//***********************************
//*****intelligence artificielle*****
//***********************************
    if(tour_tie1==300)
    {      if(hasard<5)
           {tour_tie1=10300;}
           
           if(hasard>=5)
           {tour_tie1=20300;}
    }
//***********************************
//***********************************
//***********************************    

//==================================================
//=====si le X-Wing est plus en haut de l'écran=====
//==================================================

     if(tour_tie1>=10300&&tour_tie1<10360)
           {y_tie1--;
    direction_tie1=1;}


     if(tour_tie1>=10360&&tour_tie1<10420)
           {x_tie1--;
    direction_tie1=4;}


     if(tour_tie1>=10420&&tour_tie1<10510)
           {y_tie1++;
    direction_tie1=3;}


     if(tour_tie1>=10510&&tour_tie1<10570)
           {x_tie1--;
    direction_tie1=4;}


     if(tour_tie1>=10570&&tour_tie1<10810)
           {y_tie1--;
    direction_tie1=1;}


     if(tour_tie1>=10810&&tour_tie1<11110)
           {x_tie1++;
    direction_tie1=2;}



     if(tour_tie1>=11110&&tour_tie1<11260)
           {y_tie1++;
    direction_tie1=3;}


     if(tour_tie1>=11260&&tour_tie1<11290)
           {x_tie1++;
    direction_tie1=2;}


     if(tour_tie1>=11290&&tour_tie1<11350)
           {y_tie1++;
    direction_tie1=3;}


     if(tour_tie1>=11350&&tour_tie1<11440)
           {x_tie1--;
    direction_tie1=4;}


     if(tour_tie1>=11440&&tour_tie1<11530)
           {y_tie1++;
    direction_tie1=3;}


     if(tour_tie1>=11530&&tour_tie1<11560)
           {y_tie1--;
    direction_tie1=1;}

     if(tour_tie1>=11560&&tour_tie1<11590)
           {x_tie1--;
    direction_tie1=4;}

     if(tour_tie1>=11590&&tour_tie1<11620)
           {x_tie1++;
    direction_tie1=2;}

     if(tour_tie1>=11620&&tour_tie1<11680)
           {y_tie1--;
    direction_tie1=1;}

//=============================================
//=====fin du premier parcours auxilliaire=====
//=============================================    

//=================================================
//=====si le X-Wing est plus en bas de l'écran=====
//=================================================

     if(tour_tie1>=20330&&tour_tie1<20390)
           {y_tie1--;
           direction_tie1=1;}


     if(tour_tie1>=20390&&tour_tie1<20450)
           {x_tie1--;
           direction_tie1=4;}


     if(tour_tie1>=20450&&tour_tie1<20690)
           {y_tie1++;
           direction_tie1=3;}


     if(tour_tie1>=20690&&tour_tie1<20750)
           {x_tie1--;
           direction_tie1=4;}


     if(tour_tie1>=20750&&tour_tie1<21140)
           {y_tie1--;
           direction_tie1=1;}


     if(tour_tie1>=21140&&tour_tie1<21320)
           {x_tie1++;
           direction_tie1=2;}


     if(tour_tie1>=21320&&tour_tie1<21380)
           {y_tie1++;
           direction_tie1=3;}


     if(tour_tie1>=21380&&tour_tie1<21410)
           {x_tie1++;
           direction_tie1=2;}


     if(tour_tie1>=21410&&tour_tie1<21440)
           {x_tie1--;
           direction_tie1=4;}


     if(tour_tie1>=21440&&tour_tie1<21530)
           {y_tie1++;
           direction_tie1=3;}
     
     if(tour_tie1>=21530&&tour_tie1<21560)
           {y_tie1--;
           direction_tie1=1;}
           
     if(tour_tie1>=21560&&tour_tie1<21620)
           {x_tie1++;
           direction_tie1=2;}

     if(tour_tie1>=21620&&tour_tie1<21710)
           {y_tie1++;
           direction_tie1=3;}



     
//==============================================
//=====fin du deuxieme parcours auxilliaire=====
//==============================================    

     if(tour_tie1==11680)
     {tour_tie1=1710;}

     if(tour_tie1==21710)
     {tour_tie1=1710;}

//%%%%%après être réaxé sur la routine de base%%%%%

//***********************************
//*****fin de la routine de base*****
//***********************************


     if(tour_tie1>=1710&&tour_tie1<1830)
           {x_tie1--;
           direction_tie1=4;}

     if(tour_tie1>=1830&&tour_tie1<2010)
           {y_tie1++;
           direction_tie1=3;}

     if(tour_tie1>=2010&&tour_tie1<2130)
           {x_tie1++;
           direction_tie1=4;}

//#############
tour_tie1++;
//#############

if(tour_tie1==2130)
{tour_tie1=1;}
//****************************************************************
//*****fin du parcours de Tie 1 après être réaxé sue le debut*****
//****************************************************************

return 0;}


//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§


int deplacement_tie2(void)
{


    //temps d'attente
    
    if(tour_tie2<=0)
    {direction_tie2=1;}
    
    //*************************
    //*****routine de base*****
    //*************************
    
    
    if(tour_tie2>=0&&tour_tie2<90)
    {y_tie2--;
    direction_tie2=1;}
      
    
    if(tour_tie2>=90&&tour_tie2<150)
    {x_tie2--;
    direction_tie2=4;}
      
    
    if(tour_tie2>=150&&tour_tie2<180)
    {y_tie2++;
    direction_tie2=3;}
      
    
    if(tour_tie2>=180&&tour_tie2<270)
    {x_tie2--;
    direction_tie2=4;}
    
    
    
    
//***********************************
//*****intelligence artificielle*****
//***********************************
    if(tour_tie2==270)
    {      if(hasard<5)
           {tour_tie2=10270;}
           
           if(hasard>=5)
           {tour_tie2=20270;}
    }
//***********************************
//***********************************
//***********************************    

//==================================================
//=====si le X-Wing est plus en haut de l'écran=====
//==================================================

     if(tour_tie2>=10300&&tour_tie2<10330)
           {y_tie2--;
    direction_tie2=1;}


     if(tour_tie2>=10330&&tour_tie2<10510)
           {x_tie2--;
    direction_tie2=4;}


     if(tour_tie2>=10510&&tour_tie2<10570)
           {x_tie2++;
    direction_tie2=2;}

 
     if(tour_tie2>=10570&&tour_tie2<10690)
           {y_tie2++;
    direction_tie2=3;}


     if(tour_tie2>=10690&&tour_tie2<10810)
           {x_tie2++;
    direction_tie2=2;}


 
     if(tour_tie2>=10810&&tour_tie2<10840)
           {y_tie2--;
    direction_tie2=1;}


//=============================================
//=====fin du premier parcours auxilliaire=====
//=============================================    

//=================================================
//=====si le X-Wing est plus en bas de l'écran=====
//=================================================

     if(tour_tie2>=20270&&tour_tie2<20360)
           {y_tie2++;
    direction_tie2=3;}


     if(tour_tie2>=20360&&tour_tie2<20420)
           {x_tie2--;
    direction_tie2=4;}


     if(tour_tie2>=20420&&tour_tie2<20510)
           {y_tie2++;
    direction_tie2=3;}


     if(tour_tie2>=20510&&tour_tie2<20600)
           {x_tie2++;
    direction_tie2=2;}


     if(tour_tie2>=20600&&tour_tie2<20660)
           {y_tie2--;
    direction_tie2=1;}


     if(tour_tie2>=20660&&tour_tie2<20690)
           {x_tie2--;
    direction_tie2=4;}


     if(tour_tie2>=20690&&tour_tie2<20720)
           {y_tie2--;
    direction_tie2=1;}


     if(tour_tie2>=20720&&tour_tie2<20750)
           {x_tie2--;
    direction_tie2=4;}


     if(tour_tie2>=20750&&tour_tie2<20780)
           {x_tie2++;
    direction_tie2=2;}


     if(tour_tie2>=20780&&tour_tie2<20810)
           {y_tie2--;
    direction_tie2=1;}
     

//==============================================
//=====fin du deuxieme parcours auxilliaire=====
//==============================================    

     if(tour_tie2==10840)
     {tour_tie2=810;}

     if(tour_tie2==20810)
     {tour_tie2=810;}

//%%%%%après être réaxé sur la routine de base%%%%%

//***********************************
//*****fin de la routine de base*****
//***********************************


     if(tour_tie2>=810&&tour_tie2<900)
           {x_tie2++;
    direction_tie2=2;}


     if(tour_tie2>=900&&tour_tie2<1020)
           {y_tie2++;
    direction_tie2=3;}


     if(tour_tie2>=1020&&tour_tie2<1110)
           {y_tie2--;
    direction_tie2=1;}


     if(tour_tie2>=1110&&tour_tie2<1170)
           {x_tie2++;
    direction_tie2=2;}


     if(tour_tie2>=1170&&tour_tie2<1200)
           {y_tie2--;
    direction_tie2=1;}



//#############
tour_tie2++;
//#############

if(tour_tie2==1200)
{tour_tie2=1;}
//****************************************************************
//*****fin du parcours de Tie 2 après être réaxé sue le debut*****
//****************************************************************

return 0;}

//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§


int deplacement_tie3(void)
{


    //temps d'attente
    
    if(tour_tie3<=0)
    {}
    
    //*************************
    //*****routine de base*****
    //*************************
    
    
    if(tour_tie3>=0&&tour_tie3<210)
    {y_tie3++;
    direction_tie3=3;}
      
    
    if(tour_tie3>=210&&tour_tie3<360)
    {x_tie3--;
    direction_tie3=4;}
      
    
    if(tour_tie3>=360&&tour_tie3<480)
    {y_tie3--;
    direction_tie3=1;}
      
    
    if(tour_tie3>=480&&tour_tie3<570)
    {x_tie3++;
    direction_tie3=2;}
    

    if(tour_tie3>=570&&tour_tie3<720)
    {y_tie3--;
    direction_tie3=1;}
      
    
    
//***********************************
//*****intelligence artificielle*****
//***********************************
    if(tour_tie3==720)
    {      if(hasard<5)
           {tour_tie3=10720;}
           
           if(hasard>=5)
           {tour_tie3=20720;}
    }
//***********************************
//***********************************
//***********************************    

//==================================================
//=====si le X-Wing est plus en haut de l'écran=====
//==================================================

     if(tour_tie3>=10720&&tour_tie3<10750)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=10750&&tour_tie3<10840)
           {x_tie3--;
    direction_tie3=4;}


     if(tour_tie3>=10840&&tour_tie3<10870)
           {y_tie3--;
    direction_tie3=1;}

 
     if(tour_tie3>=10870&&tour_tie3<10900)
           {x_tie3--;
    direction_tie3=4;}


     if(tour_tie3>=10900&&tour_tie3<10930)
           {x_tie3++;
    direction_tie3=2;}

 
     if(tour_tie3>=10930&&tour_tie3<10960)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=10960&&tour_tie3<11050)
           {x_tie3++;
    direction_tie3=2;}


     if(tour_tie3>=11050&&tour_tie3<11080)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=11080&&tour_tie3<11140)
           {x_tie3++;
    direction_tie3=2;}

 
     if(tour_tie3>=11140&&tour_tie3<11260)
           {y_tie3++;
    direction_tie3=3;}


     if(tour_tie3>=11260&&tour_tie3<11290)
           {x_tie3--;
    direction_tie3=4;}

 
     if(tour_tie3>=11290&&tour_tie3<11320)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=11320&&tour_tie3<11350)
           {x_tie3--;
    direction_tie3=4;}

 
     if(tour_tie3>=11350&&tour_tie3<11380)
           {y_tie3++;
    direction_tie3=3;}

//=============================================
//=====fin du premier parcours auxilliaire=====
//=============================================    

//=================================================
//=====si le X-Wing est plus en bas de l'écran=====
//=================================================

     if(tour_tie3>=20720&&tour_tie3<20750)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=20780&&tour_tie3<20870)
           {x_tie3--;
    direction_tie3=4;}


     if(tour_tie3>=20870&&tour_tie3<20900)
           {y_tie3++;
    direction_tie3=3;}


     if(tour_tie3>=20900&&tour_tie3<20960)
           {x_tie3--;
    direction_tie3=4;}


     if(tour_tie3>=20960&&tour_tie3<21050)
           {y_tie3++;
    direction_tie3=3;}


     if(tour_tie3>=21050&&tour_tie3<21140)
           {x_tie3++;
    direction_tie3=2;}


     if(tour_tie3>=21140&&tour_tie3<21200)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=21200&&tour_tie3<21230)
           {x_tie3--;
    direction_tie3=4;}


     if(tour_tie3>=21230&&tour_tie3<21290)
           {y_tie3--;
    direction_tie3=1;}


     if(tour_tie3>=21290&&tour_tie3<21380)
           {x_tie3++;
    direction_tie3=2;}
     

     if(tour_tie3>=21380&&tour_tie3<21410)
           {y_tie3++;
    direction_tie3=3;}
   
//==============================================
//=====fin du deuxieme parcours auxilliaire=====
//==============================================    

     if(tour_tie3==11380)
     {tour_tie3=1410;}

     if(tour_tie3==21410)
     {tour_tie3=1410;}

//%%%%%après être réaxé sur la routine de base%%%%%

//***********************************
//*****fin de la routine de base*****
//***********************************


     if(tour_tie3>=1410&&tour_tie3<1620)
           {y_tie3++;
    direction_tie3=3;}


     if(tour_tie3>=1620&&tour_tie3<1710)
           {x_tie3--;
    direction_tie3=4;}


     if(tour_tie3>=1710&&tour_tie3<1770)
           {y_tie3++;
    direction_tie3=3;}


     if(tour_tie3>=1770&&tour_tie3<1920)
           {x_tie3++;
    direction_tie3=2;}


     if(tour_tie3>=1920&&tour_tie3<2130)
           {y_tie3--;
    direction_tie3=1;}



//#############
tour_tie3++;

//#############

if(tour_tie3==2130)
{tour_tie3=1;}
//****************************************************************
//*****fin du parcours de Tie 3 après être réaxé sue le debut*****
//****************************************************************

return 0;}




int deplacement_imp1(void)
{



if(tour_imp1>=1000&&tour_imp1<1060)
        {x_imp1--;
        direction_imp1=4;}

if(tour_imp1>=1060&&tour_imp1<1120)
        {y_imp1--;
        direction_imp1=1;}

if(tour_imp1>=1120&&tour_imp1<1180)
        {x_imp1--;
        direction_imp1=4;}

if(tour_imp1>=1180&&tour_imp1<1240)
        {y_imp1--;
        direction_imp1=1;}
        
if(tour_imp1>=1240&&tour_imp1<1300)
        {x_imp1--;
        direction_imp1=4;}

//*****intelligence artificielle*****

    if(tour_imp1==1300)
    {            
           if(pos_x<260&&hasard<5)
           {tour_imp1=3000;}
           
          if(pos_x<260&&hasard>=5)
           {tour_imp1=2000;}
           
           if(pos_x>=260&&pos_y>=180)
           {tour_imp1=7000;}
           
           if(pos_x>=260&&pos_y<180)
           {tour_imp1=4000;}
    }
//***********************************
//***********************************




if(tour_imp1>=2000&&tour_imp1<2120)
        {y_imp1--;
        direction_imp1=1;}

if(tour_imp1>=2120&&tour_imp1<2300)
        {x_imp1--;
        direction_imp1=4;}

if(tour_imp1>=2300&&tour_imp1<2510)
        {y_imp1++;
        direction_imp1=3;}

if(tour_imp1>=2480&&tour_imp1<2660)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=2660&&tour_imp1<2750)
        {y_imp1--;
        direction_imp1=1;}

//*****intelligence artificielle***** 

if(tour_imp1==2750) 
           {if(pos_x<260&&hasard<5)
           {tour_imp1=2000;}

          if(pos_x<260&&hasard>=5)
           {tour_imp1=3000;}
           
           if(pos_x>=260&&pos_y>=180)
           {tour_imp1=7000;}
           
           if(pos_x>=260&&pos_y<180)
           {tour_imp1=4000;}
           }
           

//***********************************
//***********************************



if(tour_imp1>=3000&&tour_imp1<3270)
        {y_imp1++;
        direction_imp1=3;}

if(tour_imp1>=3270&&tour_imp1<3450)
        {x_imp1--;
        direction_imp1=4;}

if(tour_imp1>=3450&&tour_imp1<3600)
        {y_imp1--;
        direction_imp1=1;}

if(tour_imp1>=3600&&tour_imp1<3780)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=3780&&tour_imp1<3900)
        {y_imp1--;
        direction_imp1=1;}

//*****intelligence artificielle***** 

if(tour_imp1==3900) 
           {if(pos_x<260&&hasard<5)
           {tour_imp1=2000;}

          if(pos_x<260&&hasard>=5)
           {tour_imp1=3000;}
           
           if(pos_x>=260&&pos_y>=180)
           {tour_imp1=7000;}
           
           if(pos_x>=260&&pos_y<180)
           {tour_imp1=4000;}
           }

//***********************************
//***********************************



if(tour_imp1>=4000&&tour_imp1<4060)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=4060&&tour_imp1<4180)
        {y_imp1--;
        direction_imp1=1;}

if(tour_imp1>=4180&&tour_imp1<4300)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=4300&&tour_imp1<4360)
        {y_imp1++;
        direction_imp1=3;}



//*****intelligence artificielle***** 

if(tour_imp1==4360) 
           {if(hasard<5)
           {tour_imp1=5000;}

          if(hasard>=5)
           {tour_imp1=6000;}}

//***********************************
//***********************************



if(tour_imp1>=5000&&tour_imp1<5090)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=5090&&tour_imp1<5150)
        {y_imp1++;
        direction_imp1=3;}

if(tour_imp1>=5150&&tour_imp1<5300)
        {x_imp1--;
        direction_imp1=4;}

if(tour_imp1>=5300&&tour_imp1<5360)
        {y_imp1--;
        direction_imp1=1;}

if(tour_imp1>=5360&&tour_imp1<5540)
        {y_imp1++;
        direction_imp1=3;}

if(tour_imp1>=5540&&tour_imp1<5600)
        {x_imp1++;
        direction_imp1=2;}



//*****intelligence artificielle***** 

if(tour_imp1==5600) 
           {tour_imp1=1000;}

//***********************************
//***********************************


if(tour_imp1>=6000&&tour_imp1<6090)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=6090&&tour_imp1<6270)
        {y_imp1++;
        direction_imp1=3;}



if(tour_imp1>=6270&&tour_imp1<6360)
        {x_imp1--;
        direction_imp1=4;}




//*****intelligence artificielle***** 

if(tour_imp1==6360) 
           {tour_imp1=1000;}

//***********************************
//***********************************


if(tour_imp1>=7000&&tour_imp1<7060)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=7060&&tour_imp1<7260)
        {y_imp1++;
        direction_imp1=3;}

if(tour_imp1>=7260&&tour_imp1<7380)
        {x_imp1++;
        direction_imp1=2;}

if(tour_imp1>=7380&&tour_imp1<7460)
        {y_imp1--;
        direction_imp1=1;}



//*****intelligence artificielle***** 

if(tour_imp1==7460) 
           {tour_imp1=1000;}

//***********************************
//***********************************



tour_imp1++;

if(tour_imp1>=1000){
tour_position_imp1++;
if(tour_position_imp1>24)tour_position_imp1=1;}
return 0;}


int deplacement_imp2(void)
{


if(tour_imp2>=1000&&tour_imp2<1060)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=1060&&tour_imp2<1120)
       {y_imp2--;
       direction_imp2=1;}
       
//*****intelligence artificielle***** 

if(tour_imp2==1120) 
           {if(hasard<=3)tour_imp2=3000;
           if(hasard<=7&&hasard>3)tour_imp2=4000;
           if(hasard>7)tour_imp2=5000;}

//***********************************
//***********************************


if(tour_imp2>=2000&&tour_imp2<2060)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=2060&&tour_imp2<2120)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=2120&&tour_imp2<2210)
       {x_imp2--;
       direction_imp2=4;}

      
       
//*****intelligence artificielle***** 

if(tour_imp2==2210) 
           {if(hasard<=2)tour_imp2=6000;
           if(hasard<=5&&hasard>2)tour_imp2=7000;
           if(hasard<=8&&hasard>5)tour_imp2=8000;
           if(hasard>8)tour_imp2=9000;
           }

//***********************************
//***********************************



if(tour_imp2>=3000&&tour_imp2<3150)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=3150&&tour_imp2<3210)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=3210&&tour_imp2<3270)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=3270&&tour_imp2<3330)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=3330&&tour_imp2<3480)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=3480&&tour_imp2<3660)
       {y_imp2++;
       direction_imp2=3;}


       
       
       
//*****intelligence artificielle***** 

if(tour_imp2==3660) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;}

//***********************************
//***********************************



if(tour_imp2>=4000&&tour_imp2<4060)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=4060&&tour_imp2<4210)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=4210&&tour_imp2<4270)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=4270&&tour_imp2<4420)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=4420&&tour_imp2<4480)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=4480&&tour_imp2<4540)
       {x_imp2++;
       direction_imp2=2;}

       
       
       
//*****intelligence artificielle***** 

if(tour_imp2==4540) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;
           }

//***********************************
//***********************************




if(tour_imp2>=5000&&tour_imp2<5060)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=5060&&tour_imp2<5150)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=5150&&tour_imp2<5210)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=5210&&tour_imp2<5330)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=5330&&tour_imp2<5510)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=5510&&tour_imp2<5570)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=5570&&tour_imp2<5630)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=5630&&tour_imp2<5780)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=5780&&tour_imp2<5840)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=5840&&tour_imp2<5900)
       {x_imp2++;
       direction_imp2=2;}
       
       
       
//*****intelligence artificielle***** 

if(tour_imp2==5900) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;
           }

//***********************************
//***********************************



if(tour_imp2>=6000&&tour_imp2<6060)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=6060&&tour_imp2<6210)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=6210&&tour_imp2<6330)
       {y_imp2--;
       direction_imp2=1;}

     
       
       
//*****intelligence artificielle***** 

if(tour_imp2==6330) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;
           }

//***********************************
//***********************************



if(tour_imp2>=7000&&tour_imp2<7060)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=7060&&tour_imp2<7120)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=7120&&tour_imp2<7180)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=7180&&tour_imp2<7320)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=7320&&tour_imp2<7590)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=7590&&tour_imp2<7730)
       {y_imp2--;
       direction_imp2=1;}
     
       
       
//*****intelligence artificielle***** 

if(tour_imp2==7730) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;
           }

//***********************************
//***********************************



if(tour_imp2>=8000&&tour_imp2<8080)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=8080&&tour_imp2<8200)
       {x_imp2--;
       direction_imp2=4;}

if(tour_imp2>=8200&&tour_imp2<8340)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=8340&&tour_imp2<8400)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=8400&&tour_imp2<8460)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=8460&&tour_imp2<8610)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=8610&&tour_imp2<8670)
       {y_imp2++;
       direction_imp2=3;}

if(tour_imp2>=8670&&tour_imp2<8730)
       {x_imp2++;
       direction_imp2=2;}     
       
       
//*****intelligence artificielle***** 

if(tour_imp2==8730) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;
           }

//***********************************
//***********************************



if(tour_imp2>=9000&&tour_imp2<9090)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=9090&&tour_imp2<9150)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=9150&&tour_imp2<9210)
       {x_imp2++;
       direction_imp2=2;}

if(tour_imp2>=9210&&tour_imp2<9360)
       {y_imp2--;
       direction_imp2=1;}

if(tour_imp2>=9360&&tour_imp2<9510)
       {y_imp2++;
       direction_imp2=3;}


       
       
//*****intelligence artificielle***** 

if(tour_imp2==9510) 
           {if(pos_y>=240)tour_imp2=2000;
           if(pos_y<240)tour_imp2=1000;
           }

//***********************************
//***********************************




tour_imp2++;
tour_position_imp2++;
if(tour_position_imp2>24)tour_position_imp2=1;
return 0;}



int deplacement_imp3()
{

if(tour_imp3>=1000&&tour_imp3<1180)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=1180&&tour_imp3<1330)
       {x_imp3--;
       direction_imp3=4;}



       
       
//*****intelligence artificielle***** 

if(tour_imp3==1330) 
           {if(hasard<=1)tour_imp3=3000;
           if(hasard<=2&&hasard>1)tour_imp3=4000;
           if(hasard<=3&&hasard>2)tour_imp3=5000;
           if(hasard<=4&&hasard>3)tour_imp3=6000;
           if(hasard<=5&&hasard>4)tour_imp3=7000;
           if(hasard<=6&&hasard>5)tour_imp3=8000;
           if(hasard<=8&&hasard>6)tour_imp3=9000;
           if(hasard>8)tour_imp3=10000;
           }

//***********************************
//***********************************


if(tour_imp3>=2000&&tour_imp3<2140)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=2140&&tour_imp3<2290)
       {x_imp3--;
       direction_imp3=4;}



       
       
//*****intelligence artificielle***** 

if(tour_imp3==2290) 
           {if(hasard<=4)tour_imp3=11000;
           if(hasard<=7&&hasard>4)tour_imp3=12000;
           if(hasard>7)tour_imp3=13000;
           }

//***********************************
//***********************************


if(tour_imp3>=3000&&tour_imp3<3060)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=3060&&tour_imp3<3150)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=3150&&tour_imp3<3350)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=3350&&tour_imp3<3440)
       {x_imp3--;
       direction_imp3=4;}

if(tour_imp3>=3440&&tour_imp3<3500)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=3500&&tour_imp3<3650)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=3650&&tour_imp3<3790)
       {y_imp3--;
       direction_imp3=1;}


       
       
//*****intelligence artificielle***** 

if(tour_imp3==3790) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************



if(tour_imp3>=4000&&tour_imp3<4060)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=4060&&tour_imp3<4150)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=4150&&tour_imp3<4270)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=4270&&tour_imp3<4330)
       {x_imp3++;
       direction_imp3=2;}



       
       
//*****intelligence artificielle***** 

if(tour_imp3==4330) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************




if(tour_imp3>=5000&&tour_imp3<5060)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=5060&&tour_imp3<5120)
       {x_imp3--;
       direction_imp3=4;}

if(tour_imp3>=5120&&tour_imp3<5320)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=5320&&tour_imp3<5470)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=5470&&tour_imp3<5550)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=5550&&tour_imp3<5610)
       {x_imp3++;
       direction_imp3=2;}


       
       
//*****intelligence artificielle***** 

if(tour_imp3==5610) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************





if(tour_imp3>=6000&&tour_imp3<6060)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=6060&&tour_imp3<6120)
       {x_imp3--;
       direction_imp3=4;}

if(tour_imp3>=6120&&tour_imp3<6180)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=6180&&tour_imp3<6330)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=6330&&tour_imp3<6390)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=6390&&tour_imp3<6450)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=6450&&tour_imp3<6540)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=6540&&tour_imp3<6720)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=6720&&tour_imp3<6810)
       {y_imp3--;
       direction_imp3=1;}

       
       
//*****intelligence artificielle***** 

if(tour_imp3==6810) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************




if(tour_imp3>=7000&&tour_imp3<7120)
       {x_imp3--;
       direction_imp3=4;}

if(tour_imp3>=7120&&tour_imp3<7300)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=7300&&tour_imp3<7360)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=7360&&tour_imp3<7420)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=7420&&tour_imp3<7570)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=7570&&tour_imp3<7630)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=7630&&tour_imp3<7690)
       {x_imp3++;
       direction_imp3=2;}



       
       
//*****intelligence artificielle***** 

if(tour_imp3==7690) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************



if(tour_imp3>=8000&&tour_imp3<8120)
       {x_imp3--;
       direction_imp3=4;}

if(tour_imp3>=8120&&tour_imp3<8430)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=8430&&tour_imp3<8700)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=8700&&tour_imp3<8830)
       {y_imp3--;
       direction_imp3=1;}




       
       
//*****intelligence artificielle***** 

if(tour_imp3==8830) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************




if(tour_imp3>=9000&&tour_imp3<9150)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=9150&&tour_imp3<9480)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=9480&&tour_imp3<9690)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=9690&&tour_imp3<9750)
       {y_imp3++;
       direction_imp3=3;}



       
       
//*****intelligence artificielle***** 

if(tour_imp3==9750) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************




if(tour_imp3>=10000&&tour_imp3<10150)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=10150&&tour_imp3<10330)
       {y_imp3++;
       direction_imp3=3;}




       
       
//*****intelligence artificielle***** 

if(tour_imp3==10330) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************





if(tour_imp3>=11000&&tour_imp3<11060)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=11060&&tour_imp3<11150)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=11150&&tour_imp3<11230)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=11230&&tour_imp3<11290)
       {x_imp3++;
       direction_imp3=2;}




       
       
//*****intelligence artificielle***** 

if(tour_imp3==11290) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************





if(tour_imp3>=12000&&tour_imp3<12120)
       {x_imp3--;
       direction_imp3=4;}

if(tour_imp3>=12120&&tour_imp3<12390)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=12390&&tour_imp3<12530)
       {y_imp3--;
       direction_imp3=1;}





       
       
//*****intelligence artificielle***** 

if(tour_imp3==12530) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************




if(tour_imp3>=13000&&tour_imp3<13150)
       {x_imp3++;
       direction_imp3=2;}

if(tour_imp3>=13150&&tour_imp3<13450)
       {y_imp3--;
       direction_imp3=1;}

if(tour_imp3>=13450&&tour_imp3<13690)
       {y_imp3++;
       direction_imp3=3;}

if(tour_imp3>=13690&&tour_imp3<13770)
       {y_imp3--;
       direction_imp3=1;}



       
       
//*****intelligence artificielle***** 

if(tour_imp3==13770) 
           {
           if(pos_y>=240)tour_imp3=2000;
           if(pos_y<240)tour_imp3=1000;
           }

//***********************************
//***********************************




if(tour_imp3==999)
{x_imp3=590;
y_imp3=160;
}


tour_imp3++;
tour_position_imp3++;
if(tour_position_imp3>24)tour_position_imp3=1;

return 0;}


int correction_hasard(void)
{
if(hasard>10)
             {do {hasard=hasard-10;}
             while (hasard>10);}
if(hasard<1)
             {do {hasard=hasard+10;}
             while (hasard<1);}

return 0;
}






void ExitDD()
{
	if(lpDD != NULL)
	{
		if(lpDDSPrim != NULL)
		{
			lpDDSPrim->Release();
			lpDDSPrim=NULL;
		}
		if(lpDDSLogoAmanga != NULL)
		{
			lpDDSLogoAmanga->Release();
			lpDDSLogoAmanga=NULL;
		}
		if(lpDDSBack != NULL)
		{
			lpDDSLogoAmanga->Release();
			lpDDSLogoAmanga=NULL;
		}
	    if(lpDDSmenu_principal != NULL)
		{
			lpDDSmenu_principal->Release();
			lpDDSmenu_principal=NULL;
		}
    

		if(lpDDStexte != NULL)
		{
			lpDDStexte->Release();
			lpDDStexte=NULL;
		}
        if(lpDDSselect != NULL)
		{
			lpDDSselect->Release();
			lpDDSselect=NULL;
		}
		if(lpDDSfond_map1 != NULL)
		{
			lpDDSfond_map1->Release();
			lpDDSfond_map1=NULL;
		}
		if(lpDDSfond_map2 != NULL)
		{
			lpDDSfond_map2->Release();
			lpDDSfond_map2=NULL;
		}
        if(lpDDScalque_vaisseau_map2 != NULL)
		{
			lpDDScalque_vaisseau_map2->Release();
			lpDDScalque_vaisseau_map2=NULL;
		}
		if(lpDDSxwing != NULL)
		{
			lpDDSxwing->Release();
			lpDDSxwing=NULL;
		}
		if(lpDDSluke != NULL)
		{
			lpDDSluke->Release();
			lpDDSluke=NULL;
		}
        if(lpDDSimp != NULL)
		{
			lpDDSimp->Release();
			lpDDSimp=NULL;
		}
        if(lpDDStie != NULL)
		{
			lpDDStie->Release();
			lpDDStie=NULL;
		}
		if(lpDDScalque_sortie_map1 != NULL)
		{
			lpDDScalque_sortie_map1->Release();
			lpDDScalque_sortie_map1=NULL;
		}
		if(lpDDSled_map1 != NULL)
		{
			lpDDSled_map1->Release();
			lpDDSled_map1=NULL;
		}
    	if(lpDDSled_map2 != NULL)
		{
			lpDDSled_map2->Release();
			lpDDSled_map2=NULL;
		}
            if(lpDDSmenu_de_jeu != NULL)
		{
			lpDDSmenu_de_jeu->Release();
			lpDDSmenu_de_jeu=NULL;
		}
        if(lpDDSporte != NULL)
		{
			lpDDSporte->Release();
			lpDDSporte=NULL;
		}
       if(lpDDSforce != NULL)
		{
			lpDDSforce->Release();
			lpDDSforce=NULL;
		}
		if(lpDDSdv != NULL)
		{
			lpDDSdv->Release();
			lpDDSdv=NULL;
		}
        
        lpDD->Release();
		lpDD=NULL;
	}
}




