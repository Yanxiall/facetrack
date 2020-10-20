/**
 * verify username
 *
 * @param userName
 * @returns {boolean}
 */
function validUserName(userName) {
    var pattern = /^[a-zA-Z0-9_-]{4,16}$/;
    if (pattern.test(userName.trim())) {
        return (true);
    } else {
        return (false);
    }
}

/**
 * verify nickName
 *
 * @param str
 * @returns {boolean}
 */
function validNickName(NickName) {
    var pattern = /^[a-zA-Z0-9-]{2,18}$/;
    if (pattern.test(NickName.trim())) {
        return (true);
    } else {
        return (false);
    }
}
/**
 * verify password
 *
 * @param password
 * @returns {boolean}
 */
function validPassword(password) {
    var pattern = /^[a-zA-Z0-9]{6,20}$/;
    if (pattern.test(password.trim())) {
        return (true);
    } else {
        return (false);
    }
}
/**
 * Password Strength
 *
 * @param password
 *
 */
function PasswordStrength(password){

                // Do not show anything when the length of password is zero.
                if (password.length === 0) {
                    document.getElementById("msg").innerHTML = "";
                    return;
                }
                // Create an array and push all possible values that you want in password
                var matchedCase = new Array();
                matchedCase.push("[$@$!%*#?&]"); // Special Charector
                matchedCase.push("[A-Z]");      // Uppercase Alpabates
                matchedCase.push("[0-9]");      // Numbers
                matchedCase.push("[a-z]");     // Lowercase Alphabates

                // Check the conditions
                var ctr = 0;
                for (var i = 0; i < matchedCase.length; i++) {
                    if (new RegExp(matchedCase[i]).test(password)) {
                        ctr++;
                    }
                }
                // Display it
                var color = "";
                var strength = "";
                switch (ctr) {
                    case 0:
                    case 1:
                    case 2:
                        strength = "Very Weak";
                        color = "red";
                        break;
                    case 3:
                        strength = "Medium";
                        color = "orange";
                        break;
                    case 4:
                        strength = "Strong";
                        color = "green";
                        break;
                }
                document.getElementById("msg").innerHTML = strength;
                document.getElementById("msg").style.color = color;
}

function GetSelectedRow(){
    var grid = $("#jqGrid");
    var id = grid.getGridParam("selrow");
    if(id == null){
       swal("Please select one record!",{
           icon: "warning",
       });
       return;
    }
    var multiid = grid.getGridParam("selarrrow");
    if(multiid.length > 1){
       swal("More than one record are selected!",{
                  icon: "warning",
              });
              return;
    }
    return multiid[0];
}
function GetSelectedRowWithoutAlert(){
    var grid = $("#jqGrid");
    var multiid = grid.getGridParam("selarrrow");
    if(multiid == null){
        return;
    }
    if(multiid.length > 1){
         return;
    }
    return multiid[0];
}
function GetSelectedMultiRows(){
    var grid = $("#jqGrid");
    var multiid = grid.getGridParam("selarrrow");
    if(multiid.length < 1){
       swal("Please select one record!",{
              icon: "warning",
       });
       return;
    }
    return multiid;
}

/**
 * is url
 *
 * @param str
 * @returns {boolean}
 */

function validURL(str) {
  var pattern = new RegExp('^(https?:\\/\\/)?'+ // protocol
    '((([a-z\\d]([a-z\\d-]*[a-z\\d])*)\\.)+[a-z]{2,}|'+ // domain name
    '((\\d{1,3}\\.){3}\\d{1,3}))'+ // OR ip (v4) address
    '(\\:\\d+)?(\\/[-a-z\\d%_.~+]*)*'+ // port and path
    '(\\?[;&a-z\\d%_.~+=-]*)?'+ // query string
    '(\\#[-a-z\\d_]*)?$','i'); // fragment locator
  return !!pattern.test(str);
}