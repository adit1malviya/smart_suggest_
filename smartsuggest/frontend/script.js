const searchInput = document.getElementById('searchInput');
const suggestionsList = document.getElementById('suggestions');

// Input listener
searchInput.addEventListener('input', async () => {
  const query = searchInput.value.trim();
  suggestionsList.innerHTML = '';

  if (query.length === 0) {
    searchInput.style.border = '2px solid #ccc';
    suggestionsList.style.display = 'none';
    return;
  }

  try {
    const res = await fetch('http://127.0.0.1:5000/suggest', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ word: query })
    });

    const data = await res.json();
    const suggestions = data.suggestions || [];

    // 🔴 Mark red if edit-distance correction triggered
    if (data.error) {
      searchInput.style.border = '2px solid red';
    } else {
      searchInput.style.border = '2px solid #34A853'; // green
    }

    // ✅ Display suggestions with prefix highlighting
    if (suggestions.length > 0) {
      suggestionsList.innerHTML = ''; // clear old suggestions
      suggestions.forEach(suggestion => {
        const li = document.createElement('li');

        // Highlight prefix (case-insensitive)
        const regex = new RegExp(`^(${query})`, 'i');
        const highlighted = suggestion.replace(
          regex,
          '<strong style="color:#34A853;">$1</strong>'
        );

        li.innerHTML = highlighted;
        li.addEventListener('click', () => {
          searchInput.value = suggestion;
          suggestionsList.style.display = 'none';
        });
        suggestionsList.appendChild(li);
      });
      suggestionsList.style.display = 'block';
    } else {
      suggestionsList.style.display = 'none';
    }

  } catch (err) {
    console.error('Error:', err);
  }
});

// Store on Enter (refresh page after saving)
searchInput.addEventListener('keydown', async (e) => {
  if (e.key === 'Enter') {
    const word = searchInput.value.trim();
    if (word.length > 0) {
      try {
        await fetch('http://127.0.0.1:5000/store', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ word })
        });
        // Refresh the page to signal the word was saved
        window.location.reload();
      } catch (err) {
        console.error('Error saving word:', err);
      }
    }
  }
});
