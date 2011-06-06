<?php
// $Id:$

/**
 * @file convio-api-donate.tpl.php
 * Description.
 *
 * Available variables:
 * - .
 *
 * @see
 */
?>
<?php foreach ($book_menus as $book_id => $menu) : ?>
<div id="book-block-menu-<?php print $book_id; ?>" class="book-block-menu">
  <?php print $menu; ?>
</div>
<?php endforeach; ?>
