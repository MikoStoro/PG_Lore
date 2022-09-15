const defaultMode = "LIGHT";
var mode = localStorage.getItem("mode");

if(mode === null){
    mode = defaultMode;
    localStorage.setItem("mode", defaultMode);
}

function setMode(){
    if(mode == "DARK"){
        document.body.classList.add("dark");
        document.getElementById("darkmodeBtn").textContent = "Tryb Jasny";
    }
    if(mode =="LIGHT"){
        document.body.classList.remove("dark");
        document.getElementById("darkmodeBtn").textContent = "Tryb Ciemny";
    }

    $("#darkmodeBtn").button({
        icon: "ui-icon-lightbulb"
    })
}

function changeMode(){
    
    if(mode == "DARK"){
        mode = "LIGHT";
        localStorage.setItem("mode", "LIGHT");
    }else{
        mode = "DARK";
        localStorage.setItem("mode", "DARK");
    }
    setMode();
}