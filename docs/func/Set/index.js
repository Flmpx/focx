// Set 模块索引页面脚本
document.addEventListener('DOMContentLoaded', function() {
    if (typeof Prism !== 'undefined') {
        Prism.highlightAll();
    } else {
        setTimeout(function() {
            if (typeof Prism !== 'undefined') {
                Prism.highlightAll();
            }
        }, 500);
    }

    const cards = document.querySelectorAll('.moduleCard');
    cards.forEach((card, index) => {
        card.style.opacity = '0';
        card.style.transform = 'translateY(20px)';
        setTimeout(() => {
            card.style.transition = 'all 0.4s ease';
            card.style.opacity = '1';
            card.style.transform = 'translateY(0)';
        }, 100 * index);
    });
});
