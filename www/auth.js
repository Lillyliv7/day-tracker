async function pageOpenCheck() {
    let response = await checkToken();
    if (!response) {
        alert("token expired, please login again");
        window.location.href = "login.html";
    }
}

async function checkToken() {
    let postData = {
        type: "account",
        operation: "check_token",
        username: localStorage.getItem("username"),
        token: localStorage.getItem("token"),
    }
    try {
        const response = await fetch(localStorage.getItem("server"), {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json; charset=UTF-8',
            },
            body: JSON.stringify(postData),
        })
        return (response.status == 200)
    } catch {
        return false;
    }
}

function login(server, username, password) {
    if (server === undefined ||
        username === undefined ||
        password === undefined) {
            alert("must fill all boxes");
        }

    let postData = {
        type: "account",
        operation: "auth",
        username: username,
        password: password,
    }

    fetch(server, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json; charset=UTF-8',
        },
        body: JSON.stringify(postData),
    })
    .then(async response => {
        if (response.status == 200) {
            let responseJson = await response.json();
            console.log(responseJson);
            if (await responseJson.token === undefined) {
                alert("server sent invalid data");
                return;
            }
            localStorage.setItem("username", postData.username);
            localStorage.setItem("token", responseJson.token);
            localStorage.setItem("server", document.getElementById("server").value)
            window.location.href = "index.html";
        }
        else if (response.status == 401) {
            alert("invalid username or password");
        } else {
            alert("other error");
        }
    })
}