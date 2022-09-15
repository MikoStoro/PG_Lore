var lastElement = null;





function bookmark(elementId){
    var elem = document.getElementById(elementId);
    var tempStr = elem.innerHTML;
    tempStr = "> " + tempStr;
    elem.innerHTML = tempStr;
    elem.scrollIntoView();
    
    if(lastElement != null){
        console.log(lastElement.innerHTML);
        lastElement.innerHTML = lastElement.innerHTML.substring(4);
    }

    sessionStorage.setItem("bookmark", elementId);
    lastElement = elem;
}   

function init(){
    if(sessionStorage.getItem("bookmark")){
            bookmark(sessionStorage.getItem("bookmark"));
        }

}