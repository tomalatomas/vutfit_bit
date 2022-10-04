const showMobileMenu = () =>{
    const navigationButton = document.getElementById('navButton')
    const navigationList = document.querySelectorAll('.navMenu')[0]

    if(navigationButton && navigationList){
        navigationButton.addEventListener('click', ()=>{
            navigationList.classList.toggle('showMobileMenu')
        })
    }
}

/*==================== REMOVE MENU MOBILE ====================*/

function linkAction(){
    const navMenu = document.querySelectorAll('.navMenu')[0]
    navMenu.classList.remove('showMobileMenu')
}


showMobileMenu()

const navLink = document.querySelectorAll('.navLink')
navLink.forEach(n => n.addEventListener('click', linkAction))
