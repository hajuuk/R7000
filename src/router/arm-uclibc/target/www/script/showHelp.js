<!-- 

function setHelpIframeVisible(){
    $('.help-frame-div').css("visibility","visible");
}


function showHelpIframe() {
    var imgSrc=document.getElementById('help-button');

    if(imgSrc.src.search("up")>=0)
    {
        $(".help-frame-div").show();
        imgSrc.src="img/helparrowdown-icon.gif";
    }
    else
    {
        $(".help-frame-div").hide();
        imgSrc.src="img/helparrowup-icon.gif";
        setTimeout(setHelpIframeVisible,500);
    }
         change_size();

}

function moveToHTMLend()
{
    window.location.href='#helpframe-anchor';
    setHelpIframeVisible();
}


function loadhelp(fname,anchname)
{
    var pane = window.frames["helpframe"].$('#content');
    var imgSrc=document.getElementById('help-button');
    if(imgSrc.src.search("up")<0)
    {

        $(".help-frame-div").show();
        pane.jScrollPane({showArrows:true});


        if ((loadhelp.arguments.length == 1 ) || (anchname == "" ))
        {
                window.frames["helpframe"].location.href=fname+"_h.htm";
                $(".help-frame-div").show();
        }
        else
        {
                window.frames["helpframe"].location.href=fname+"_h.htm#" + anchname;
                $(".help-frame-div").show();
        }

        $(".help-frame-div").show();
        pane.jScrollPane({showArrows:true});

    }
}

//-->