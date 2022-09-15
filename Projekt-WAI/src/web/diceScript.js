var publicDice = [12]
var publicDiceValue = 12;

function updateDiceDisplay(){
    var diceValue = $('#diceRange').slider("option", "value");
    //var diceValue = document.getElementById("diceRange").value;
    const dice = [];
    var resStr = "";
    publicDiceValue = diceValue;
    
    if(diceValue%12==0){
        for(var i=0; i < diceValue/12; i++){
            dice.push(12)
        }
    }else{
        for(var i = 10; i > 0; i -= 2){
            while((diceValue - i) >= 0){
                dice.push(i);
                diceValue -= i;
            }
        }
    }

    for(var i = 0; i < dice.length-1; i++){
        var temp = "K" + dice[i].toString() + " + ";
        resStr += temp;
    }

    resStr += ("K" + dice[(dice.length)-1].toString());

    document.getElementById("diceDisplay").innerHTML = resStr;

    publicDice = dice;
    
}

function roll(){
    var res = 0;
    for(var i = 0; i < publicDice.length; i++){
        res += Math.floor((Math.random()*publicDice[i])+1)
    }
    
    var row = document.createElement("TR");
    var cell = document.createElement("TD");
    
    cell.innerHTML = res;
    row.appendChild(cell);

    cell = document.createElement("TD");
    cell.innerHTML = "K" + publicDiceValue;
    row.appendChild(cell);

    tab = document.getElementById("rollTab");
    tab.insertBefore(row, tab.children[1]);
    
    row.style.backgroundColor = "#ffa929";
    setTimeout(function(){
        row.style.backgroundColor = "white";
    }, 250);
}