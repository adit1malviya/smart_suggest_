const searchInput = document.getElementById('searchInput');
const suggestionsList = document.getElementById('suggestions');

// Dummy suggestions (replace with backend API)
const smartSuggestions = [
  "Apple",
  "Application",
  "Apricot",
  "Banana",
  "Blueberry",
  "Blackberry",
  "Cherry",
  "Date",
  "Dragonfruit",
  "Grape",
  "Grapefruit",
  "Mango",
  "Melon"
];

searchInput.addEventListener('input', () => {
  const query = searchInput.value.toLowerCase();
  suggestionsList.innerHTML = '';

  if (query.length === 0) {
    suggestionsList.style.display = 'none';
    return;
  }

  const filtered = smartSuggestions.filter(item => item.toLowerCase().startsWith(query));
  filtered.forEach(item => {
    const li = document.createElement('li');

    // Highlight the prefix
    li.innerHTML = `<span class="highlight">${item.substring(0, query.length)}</span>${item.substring(query.length)}`;

    li.addEventListener('click', () => {
      searchInput.value = item;
      suggestionsList.style.display = 'none';
    });
    suggestionsList.appendChild(li);
  });

  suggestionsList.style.display = filtered.length ? 'block' : 'none';
});

// Hide suggestions when clicking outside
document.addEventListener('click', (e) => {
  if (!searchInput.contains(e.target)) {
    suggestionsList.style.display = 'none';
  }
});
