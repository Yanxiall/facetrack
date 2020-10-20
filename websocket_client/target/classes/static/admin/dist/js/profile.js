$(function ()
{
  //change username
  $("#updateNameButton").click(function ()
    {
       console.log("come");
      var UserName = $("#UserName").val();
      var NickName = $("#NickName").val();
      if(validUserNameForUpdate(UserName,NickName))
      {
          $.ajax
          ({
              type: "POST",
              url: "/admin/profile/name",
              data: $("#NameForm").serialize(),
              dataType:"text",
              success: function(result)
              {
                 console.log(result);
                 if(result == 'success')
                 {
                  alert("Name Modification Succeed");
                  window.location.href = '/admin/login';
                  }
                 else
                 {
                   alert("Name Modification Fail");
                 }
              },
              error:function()
              {
                 alert("request failed");
              }
          });
      }
    });

 //change password
  $("#updatePasswordButton").click(function ()
  {
    var originalPassword = $("#originalPassword").val();
    var newPassword = $("#newPassword").val();
    if(!validPassword(newPassword))
    {
     $('#updatePassword-info').css("display", "block");
     $("#updatePassword-info").html("please input valid password!");
    }
    else
    {
        $.ajax
        ({
            type: "POST",
            url: "/admin/profile/password",
            data: $("#userPasswordForm").serialize(),
            dataType:"text",
            success: function(result)
            {
               console.log(result);
               if(result == 'success')
               {
                alert("Password Modification Succeed");
                window.location.href = '/admin/login';
                }
               else
               {
                 alert("Password Modification Fail");
               }
            },
            error:function()
            {
               alert("request failed");
            }
        });
    }
  });
})
//verify if input valid name
function validUserNameForUpdate(UserName, NickName) {
    if (!validUserName(UserName)) {
        $('#updateName-info').css("display", "block");
        $('#updateName-info').html("please input valid userName!");
        return false;
    }
    if (!validNickName(NickName)) {
        $('#updateName-info').css("display", "block");
        $('#updateName-info').html("please input valid nickName!");
        return false;
    }
    return true;
}