
/*CommDialog*/
CommDialog QPushButton{
	border-style:none;
	padding:0px;
	border-radius:5px;
	color:rgb(240,240,240);
	background:#28AA55;
	font-size: 14px;
}  
 
CommDialog QPushButton:hover{
	color:#F0F0F0;
	background:#4E6D8C;
}

CommDialog QPushButton:pressed{
	color:#B8C6D1;
	background:#2D3E50;

 	/* 使文字有一点移动 */  
    	padding-left:3px;  
    	padding-top:3px; 
}

CommDialog QPushButton:flat {  
    	border:2px solid blue;  
} 

CommDialog QPushButton:!Enabled {  
    	color:#B8C6D1;
	background:#2D1E50;
} 

CommDialog QGroupBox{
	border-style:none;
	padding:10px;
	border-radius:5px;
	color:rgb(255,255,255);
	background:#8A8A8A;
}

CommDialog QLineEdit{
	border:1px solid rgb(41, 57, 85);
	border-radius: 3px;
	background: black;
	selection-background-color: green;
	color: white;
	font-size: 14px;
}

CommDialog QSlider{ 
        border:18px solid #8A8A8A;      
}

CommDialog QSlider::handle:vertical:!Enabled{
		background: #444444;  
    	width: 16px;  
    	height: 16px;   
    	border-radius:11px;   
		border: 3px solid #444444;
}

CommDialog QSlider::handle:vertical{
	background:  green;
	width: 16px;
	height: 16px;
	border - radius:11px;
	border: 3px solid green;
}


/*TestDialog*/
TestDialog{
	border-style: none;
	background: #555555;
} 

TestDialog QPushButton{
	border-style: none;
	padding: 0px;
	border-radius: 5px;
	color: rgb(255,255,255);
	background: #50AAFF;
}  

TestDialog QPushButton:hover{
	color: #F0F0F0;
	background: #4E6D8C;
}

TestDialog QPushButton:pressed{
	color: #B8C6D1;
	background: #2D3E50;
	/* 使文字有一点移动 */
	padding-left: 3px;
	padding-top: 3px;
}

TestDialog QPushButton:flat{
	border: 2px solid blue;
}

TestDialog QPushButton:!Enabled{
	color: #B8C6D1;
	background: #2D1E50;
}